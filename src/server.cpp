#include "include/server.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>
using boost::asio::ip::make_address;

ServerInstance::ServerInstance(const std::string &addr, int32_t port): 
m_Addr(addr), m_Port(port), m_AsioCTX(1), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(CLIENTS_MAX_CAPACITY) {}

ServerInstance::ServerInstance(const std::string &addr, int32_t port, Router router):
m_Router(router), m_Addr(addr), m_Port(port), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(CLIENTS_MAX_CAPACITY) {}

void ServerInstance::include_router(Router router)
{
    m_Router = router;
}

void ServerInstance::get(const std::string &path, CallbackHandler &&handler)
{
    m_Router.register_handler(
        path, HttpMethods::GET, 
        std::forward<CallbackHandler>(handler)
    );
}

void ServerInstance::post(const std::string &path, CallbackHandler &&handler)
{
    m_Router.register_handler(
        path, HttpMethods::POST, 
        std::forward<CallbackHandler>(handler)
    );
}

void ServerInstance::start()
{
    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(m_AsioCTX);
        m_Acceptor.accept(*socket);

        if (m_Pool.active_tasks_count() + 1 >= CLIENTS_MAX_CAPACITY)
            socket->close();

        m_Pool.add_task([this, socket = std::move(socket)]() {
            process_connection(std::move(socket));
        });
    }
}

void ServerInstance::process_connection(std::shared_ptr<tcp::socket> socket)
{
    Response<std::string> response;
    try
    {
        boost::asio::streambuf buffer;
        boost::asio::read_until(*socket, buffer, "\r\n\r\n");

        std::istream request_stream(&buffer);

        std::string headers;
        {
            std::ostringstream ss;
            ss << request_stream.rdbuf();
            headers = ss.str();
        }

        auto request = deserialize_request(headers);

        std::size_t content_length = 0;
        auto it = request.m_Headers.find("Content-Length");
        if (it != request.m_Headers.end())
            content_length = std::stoul(it->second);

        std::string body;

        std::size_t already_buffered = buffer.size();

        if (already_buffered > 0)
        {
            std::vector<char> tmp(already_buffered);
            request_stream.read(tmp.data(), already_buffered);
            body.append(tmp.data(), request_stream.gcount());
        }

        if (body.size() < content_length)
        {
            std::size_t remaining = content_length - body.size();
            std::vector<char> tmp(remaining);

            boost::asio::read(*socket, boost::asio::buffer(tmp));
            body.append(tmp.data(), remaining);
        }

        request.m_Body = std::move(body);

        auto handler = m_Router.get_handler(request.m_Path, request.m_Method);
        handler(request, response);

        auto response_payload = serialize_response(response);
        boost::asio::write(*socket, boost::asio::buffer(response_payload));

        // Temporary test shutdown
        boost::system::error_code ec;
        socket->shutdown(tcp::socket::shutdown_both, ec);
        socket->close(ec);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}