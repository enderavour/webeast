#include "include/server.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>
#include "include/defs.hpp"
#include "include/logger.hpp"
#include <print>
#include <format>
using boost::asio::ip::make_address;

ServerInstance::ServerInstance(const std::string &addr, int32_t port): 
m_Addr(addr), m_Port(port), m_AsioCTX(1), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(defaults::CLIENTS_MAX_CAPACITY) 
{
    logger::info(std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
}

ServerInstance::ServerInstance(const std::string &addr, int32_t port, Router router):
m_Router(router), m_Addr(addr), m_Port(port), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(defaults::CLIENTS_MAX_CAPACITY) 
{
    logger::info(std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
}

void ServerInstance::include_router(Router router)
{
    m_Router = router;
}

// Static
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

void ServerInstance::put(const std::string &path, CallbackHandler &&handler)
{
    m_Router.register_handler(
        path, HttpMethods::PUT, 
        std::forward<CallbackHandler>(handler)
    );
}

void ServerInstance::_delete(const std::string &path, CallbackHandler &&handler)
{
    m_Router.register_handler(
        path, HttpMethods::DELETE,
        std::forward<CallbackHandler>(handler)
    );
}

// Dynamic
void ServerInstance::get(const std::string &path, DynamicCallbackHandler &&handler)
{
    m_Router.register_dynamic(
        path, HttpMethods::GET, 
        std::forward<DynamicCallbackHandler>(handler)
    );
}

void ServerInstance::post(const std::string &path, DynamicCallbackHandler &&handler)
{
    m_Router.register_dynamic(
        path, HttpMethods::POST, 
        std::forward<DynamicCallbackHandler>(handler)
    );
}

void ServerInstance::put(const std::string &path, DynamicCallbackHandler &&handler)
{
    m_Router.register_dynamic(
        path, HttpMethods::PUT, 
        std::forward<DynamicCallbackHandler>(handler)
    );
}

void ServerInstance::_delete(const std::string &path, DynamicCallbackHandler &&handler)
{
    m_Router.register_dynamic(
        path, HttpMethods::DELETE,
        std::forward<DynamicCallbackHandler>(handler)
    );
}

void ServerInstance::start()
{
    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(m_AsioCTX);
        m_Acceptor.accept(*socket);

        if (m_Pool.active_tasks_count() + 1 >= defaults::CLIENTS_MAX_CAPACITY)
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
        while (true)
        {
            boost::system::error_code ec;
            boost::asio::read_until(*socket, buffer, "\r\n\r\n", ec);

            if (ec == boost::asio::error::eof)
                break;
            
            if (ec)
                throw boost::system::system_error(ec);

            std::string data(
                boost::asio::buffers_begin(buffer.data()),
                boost::asio::buffers_end(buffer.data())
            );

            auto pos = data.find("\r\n\r\n");
            if (pos == std::string::npos)
                throw std::runtime_error("Invalid HTTP request");

            std::string header_part = data.substr(0, pos + 4);
            std::string body_part = data.substr(pos + 4);

            auto request = deserialize_request(header_part);

            std::size_t content_length = 0;
            auto it = request.m_Headers.find("Content-Length");
            if (it != request.m_Headers.end())
                content_length = std::stoul(it->second);

            std::string body = body_part;

            if (body.size() < content_length)
            {
                std::vector<char> tmp(content_length - body.size());
                boost::asio::read(*socket, boost::asio::buffer(tmp));
                body.append(tmp.data(), tmp.size());
            }

            request.m_Body = std::move(body);

            buffer.consume((pos + 4) + body.size());

            boost::smatch _match;
            auto handler_pair = m_Router.get_handler(request.m_Path, request.m_Method, _match);
            CallbackHandler static_handler;
            DynamicCallbackHandler dynamic_handler;

            if (handler_pair.first == 0)
            {
                static_handler = std::get<CallbackHandler>(handler_pair.second);
                static_handler(request, response);
            }
            else
            {
                dynamic_handler = std::get<DynamicCallbackHandler>(handler_pair.second);
                dynamic_handler(request, response, _match);
            }

            response.set_header("Connection", "keep-alive");
            response.set_header("Content-Length", std::to_string(response.get_body().size()));

            auto response_payload = serialize_response(response);
            boost::asio::write(*socket, boost::asio::buffer(response_payload));
            if (ec)
            {
                if (ec == boost::asio::error::connection_reset)
                {
                    std::println("Client resetted the connection");
                }
                else
                {
                    std::println("Connection error occured: {}", ec.message());
                }
            }

            auto it_conn = request.m_Headers.find("Connection");
            if (it_conn != request.m_Headers.end() && it_conn->second == "close")
                break;
        }
        socket->close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}