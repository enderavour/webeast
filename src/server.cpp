#include "include/server.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>
#include "include/defs.hpp"
#include "include/logger.hpp"
#include <print>
#include <format>
#include <nlohmann/json.hpp>

using boost::asio::ip::make_address;
using json = nlohmann::json;

sv::server::server(const std::string &addr, int32_t port): 
m_Addr(addr), m_Port(port), m_AsioCTX(1), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(defaults::CLIENTS_MAX_CAPACITY) 
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
#endif
}

sv::server::server(const std::string &addr, int32_t port, rt::router router):
m_Router(router), m_Addr(addr), m_Port(port), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(defaults::CLIENTS_MAX_CAPACITY) 
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, defaults::CLIENTS_MAX_CAPACITY
    ));
#endif
}

void sv::server::include_router(rt::router router)
{
    m_Router = router;
}

// Static
void sv::server::get(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Get, 
        std::forward<rt::callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering GET request at path: {}", path
    ));
#elifdef LOGGING_ENABLE_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering GET request at path: {}", path
    ));
#endif
}

void sv::server::post(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Post, 
        std::forward<rt::callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering POST request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering POST request at path: {}", path
    ));
#endif
}

void sv::server::put(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Put, 
        std::forward<rt::callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering PUT request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering PUT request at path: {}", path
    ));
#endif
}

void sv::server::_delete(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Delete,
        std::forward<rt::callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering DELETE request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering DELETE request at path: {}", path
    ));
#endif
}

// Dynamic
void sv::server::get(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Get, 
        std::forward<rt::dynamic_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering dynamic GET request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering dynamic GET request at path: {}", path
    ));
#endif
}

void sv::server::post(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Post, 
        std::forward<rt::dynamic_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering dynamic POST request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering dynamic POST request at path: {}", path
    ));
#endif
}

void sv::server::put(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Put, 
        std::forward<rt::dynamic_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering dynamic PUT request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering dynamic PUT request at path: {}", path
    ));
#endif
}

void sv::server::_delete(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Delete,
        std::forward<rt::dynamic_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering dynamic DELETE request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering dynamic DELETE request at path: {}", path
    ));
#endif
}

// Json
void sv::server::get(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(        
        path, http::http_method::Json_Get,
        std::forward<rt::json_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON GET request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON GET request at path: {}", path
    ));
#endif
}

void sv::server::post(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(        
        path, http::http_method::Json_Post, 
        std::forward<rt::json_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON POST request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON POST request at path: {}", path
    ));
#endif
}

void sv::server::put(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(        
        path, http::http_method::Json_Put, 
        std::forward<rt::json_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON PUT request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON PUT request at path: {}", path
    ));
#endif
}

void sv::server::_delete(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(        
        path, http::http_method::Json_Delete, 
        std::forward<rt::json_callback_handler>(handler)
    );
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON DELETE request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON DELETE request at path: {}", path
    ));
#endif
}

// Json Dynamic
void sv::server::get(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Get,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON Dynamic GET request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON Dynamic GET request at path: {}", path
    ));
#endif
}

void sv::server::post(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Post,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON Dynamic POST request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON Dynamic POST request at path: {}", path
    ));
#endif
}

void sv::server::put(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Put,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON Dynamic PUT request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON Dynamic PUT request at path: {}", path
    ));
#endif
}

void sv::server::_delete(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Delete,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format(
        "Registering JSON Dynamic DELETE request at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format(
        "Registering JSON Dynamic DELETE request at path: {}", path
    ));
#endif
}

void sv::server::run_sync()
{
    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(m_AsioCTX);
        m_Acceptor.accept(*socket);

#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Accepted new client");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Accepted new client");
#endif

        if (m_Pool.active_tasks_count() + 1 >= defaults::CLIENTS_MAX_CAPACITY)
        {
#ifdef LOGGING_ENABLED_STDOUT
    logger::error("Thread pool capacity exceded. Closing connection.");
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, "Thread pool capacity exceded. Closing connection.");
#endif
            socket->close();
        }

        m_Pool.add_task([this, socket = std::move(socket)]() {
            process_connection(std::move(socket));
        });
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Adding processing task to thread pool");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Adding processing task to thread pool");
#endif
    }
}

boost::asio::awaitable<void> sv::server::run_async()
{
    auto executor = co_await boost::asio::this_coro::executor;
    while (true)
    {
        try 
        {
            auto socket = std::make_shared<tcp::socket>(co_await m_Acceptor.async_accept(executor));
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Accepted new client");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Accepted new client");
#endif
            boost::asio::co_spawn(executor, [this, sock = std::move(socket)]() mutable -> boost::asio::awaitable<void> 
            {
                co_await process_connection_async(sock);
            }, boost::asio::detached);
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Started coroutine of processing task");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Started coroutine of processing task");
#endif
        }
        catch (const std::exception &e)
        {
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Async accept error: {}", e.what());
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Async accept error: {}", e.what()));
#endif           
        }
    }
}

// Sync
void sv::server::process_connection(std::shared_ptr<tcp::socket> socket)
{
    http::response_builder<std::string> response;
    http::response_builder<json> j_response;
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
            {
#ifdef LOGGING_ENABLED_STDOUT
    logger::error("Could not parse invalid HTTP request.");
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, "Could not parse nvallid HTTP request.");
#endif
                throw std::runtime_error("Invalid HTTP request");
            }

            std::string header_part = data.substr(0, pos + 4);
            std::string body_part = data.substr(pos + 4);

            auto request = http::deserialize_request(header_part);

            // If headers contain Content-Type: application/json, then set 
            // appropriate json method into request
            auto cont_t = request.m_Headers.find("Content-Type");
            if (cont_t != request.m_Headers.end() && 
                cont_t->second.find("application/json") != std::string::npos)
            {
                switch (request.m_Method)
                {
                    case http::http_method::Get:
                        request.m_Method = http::http_method::Json_Get;
                        break;
                    case http::http_method::Post:
                        request.m_Method = http::http_method::Json_Post;
                        break;
                    case http::http_method::Put:
                        request.m_Method = http::http_method::Json_Put;
                        break;
                    case http::http_method::Delete:
                        request.m_Method = http::http_method::Json_Delete;
                        break;
                    default:
                        break;
                }
            }

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
            rt::callback_handler static_handler;
            rt::dynamic_callback_handler dynamic_handler;
            rt::json_callback_handler json_handler;
            rt::json_dynamic_callback_handler json_dynamic;

            if (handler_pair.first == 0)
            {
                static_handler = std::get<rt::callback_handler>(handler_pair.second);
                static_handler(request, response);
            }
            else if (handler_pair.first == 1)
            {
                dynamic_handler = std::get<rt::dynamic_callback_handler>(handler_pair.second);
                dynamic_handler(request, response, _match);
            }
            else if (handler_pair.first == 2)
            {
                json_handler = std::get<rt::json_callback_handler>(handler_pair.second);

                auto j_body = json::parse(request.m_Body);
                http::request<json> j_request{
                    request.m_Method,
                    request.m_Path,
                    request.m_HttpVersion,
                    request.m_Headers,
                    request.m_Params,
                    j_body
                };

                json_handler(j_request, j_response);
            }
            else if (handler_pair.first == 3)
            {
                json_dynamic = std::get<rt::json_dynamic_callback_handler>(handler_pair.second);

                auto j_body = json::parse(request.m_Body);
                http::request<json> j_request{
                    request.m_Method,
                    request.m_Path,
                    request.m_HttpVersion,
                    request.m_Headers,
                    request.m_Params,
                    j_body
                };

                json_dynamic(j_request, j_response, _match);
            }

            response.set_header("Connection", "keep-alive");
            response.set_header("Content-Length", std::to_string(response.get_body().size()));

            std::string response_payload;

            if (handler_pair.first == 2 || handler_pair.first == 3)
                response_payload = serialize_response(j_response);
            else
                response_payload = serialize_response(response);

            boost::asio::write(*socket, boost::asio::buffer(response_payload));
            if (ec)
            {
                if (ec == boost::asio::error::connection_reset)
                {
                    #ifdef LOGGING_ENABLED_STDOUT
                        logger::info("Client resetted the connection");
                    #elifdef LOGGING_ENABLED_FILE
                        logger::info(defaults::LOG_FILE_HANDLE, "Client resetted the connection");
                    #endif
                }
                else
                {
                    #ifdef LOGGING_ENABLED_STDOUT
                        logger::error(std::format(                            
                            "Connection error occured: {}", ec.what()
                        ));
                    #elifdef LOGGING_ENABLED_FILE
                        logger::error(defaults::LOG_FILE_HANDLE, std::format(                            
                            "Connection error occured: {}", ec.what()
                        ));
                    #endif
                }
            }

            auto it_conn = request.m_Headers.find("Connection");
            if (it_conn != request.m_Headers.end() && it_conn->second == "close")
            {
                #ifdef LOGGING_ENABLED_STDOUT
                    logger::info("Client closed the connection");
                #elifdef LOGGING_ENABLED_FILE
                    logger::info(defaults::LOG_FILE_HANDLE, "Client closed the connection");
                #endif
                break;
            }
        }
        socket->close();
    }
    catch (const std::exception &e)
    {
#ifdef LOGGING_ENABLED_STDOUT
    logger::error(std::format("Error occured: {}", e.what()));
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, std::format("Error occured: {}", e.what()));
#endif
    }
}

// Async
boost::asio::awaitable<void> sv::server::process_connection_async(std::shared_ptr<tcp::socket> socket)
{
    http::response_builder<std::string> response;
    http::response_builder<json> j_response;
    try
    {
        boost::asio::streambuf buffer;
        while (true)
        {
            boost::system::error_code ec;
            size_t n = co_await boost::asio::async_read_until(*socket, buffer, "\r\n\r\n", boost::asio::use_awaitable);

            std::string data(
                boost::asio::buffers_begin(buffer.data()),
                boost::asio::buffers_end(buffer.data())
            );

            auto pos = data.find("\r\n\r\n");
            if (pos == std::string::npos)
            {
#ifdef LOGGING_ENABLED_STDOUT
    logger::error("Could not parse invalid HTTP request.");
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, "Could not parse nvallid HTTP request.");
#endif
                throw std::runtime_error("Invalid HTTP request");
            }

            std::string header_part = data.substr(0, pos + 4);
            std::string body_part = data.substr(pos + 4);

            auto request = http::deserialize_request(header_part);

            // If headers contain Content-Type: application/json, then set 
            // appropriate json method into request
            auto cont_t = request.m_Headers.find("Content-Type");
            if (cont_t != request.m_Headers.end() && 
                cont_t->second.find("application/json") != std::string::npos)
            {
                switch (request.m_Method)
                {
                    case http::http_method::Get:
                        request.m_Method = http::http_method::Json_Get;
                        break;
                    case http::http_method::Post:
                        request.m_Method = http::http_method::Json_Post;
                        break;
                    case http::http_method::Put:
                        request.m_Method = http::http_method::Json_Put;
                        break;
                    case http::http_method::Delete:
                        request.m_Method = http::http_method::Json_Delete;
                        break;
                    default:
                        break;
                }
            }

            std::size_t content_length = 0;
            auto it = request.m_Headers.find("Content-Length");
            if (it != request.m_Headers.end())
                content_length = std::stoul(it->second);

            std::string body = body_part;

            if (body.size() < content_length)
            {
                std::vector<char> tmp(content_length - body.size());
                size_t n = co_await boost::asio::async_read(*socket, boost::asio::buffer(tmp), boost::asio::use_awaitable);
                body.append(tmp.data(), n);
            }

            request.m_Body = std::move(body);

            buffer.consume(buffer.size());

            boost::smatch _match;
            auto handler_pair = m_Router.get_handler(request.m_Path, request.m_Method, _match);
            rt::callback_handler static_handler;
            rt::dynamic_callback_handler dynamic_handler;
            rt::json_callback_handler json_handler;
            rt::json_dynamic_callback_handler json_dynamic;

            if (handler_pair.first == 0)
            {
                static_handler = std::get<rt::callback_handler>(handler_pair.second);
                static_handler(request, response);
            }
            else if (handler_pair.first == 1)
            {
                dynamic_handler = std::get<rt::dynamic_callback_handler>(handler_pair.second);
                dynamic_handler(request, response, _match);
            }
            else if (handler_pair.first == 2)
            {
                json_handler = std::get<rt::json_callback_handler>(handler_pair.second);

                auto j_body = json::parse(request.m_Body);
                http::request<json> j_request{
                    request.m_Method,
                    request.m_Path,
                    request.m_HttpVersion,
                    request.m_Headers,
                    request.m_Params,
                    j_body
                };

                json_handler(j_request, j_response);
            }
            else if (handler_pair.first == 3)
            {
                json_dynamic = std::get<rt::json_dynamic_callback_handler>(handler_pair.second);

                auto j_body = json::parse(request.m_Body);
                http::request<json> j_request{
                    request.m_Method,
                    request.m_Path,
                    request.m_HttpVersion,
                    request.m_Headers,
                    request.m_Params,
                    j_body
                };

                json_dynamic(j_request, j_response, _match);
            }

            response.set_header("Connection", "keep-alive");
            response.set_header("Content-Length", std::to_string(response.get_body().size()));

            std::string response_payload;

            if (handler_pair.first == 2 || handler_pair.first == 3)
                response_payload = serialize_response(j_response);
            else
                response_payload = serialize_response(response);

            co_await boost::asio::async_write(*socket, boost::asio::buffer(response_payload), boost::asio::use_awaitable);
            if (ec)
            {
                if (ec == boost::asio::error::connection_reset)
                {
                    #ifdef LOGGING_ENABLED_STDOUT
                        logger::info("Client resetted the connection");
                    #elifdef LOGGING_ENABLED_FILE
                        logger::info(defaults::LOG_FILE_HANDLE, "Client resetted the connection");
                    #endif
                }
                else
                {
                    #ifdef LOGGING_ENABLED_STDOUT
                        logger::error(std::format(                            
                            "Connection error occured: {}", ec.what()
                        ));
                    #elifdef LOGGING_ENABLED_FILE
                        logger::error(defaults::LOG_FILE_HANDLE, std::format(                            
                            "Connection error occured: {}", ec.what()
                        ));
                    #endif
                }
            }

            auto it_conn = request.m_Headers.find("Connection");
            if (it_conn != request.m_Headers.end() && it_conn->second == "close")
            {
                #ifdef LOGGING_ENABLED_STDOUT
                    logger::info("Client closed the connection");
                #elifdef LOGGING_ENABLED_FILE
                    logger::info(defaults::LOG_FILE_HANDLE, "Client closed the connection");
                #endif
                break;
            }
        }
        socket->close();
    }
    catch (const std::exception &e)
    {
#ifdef LOGGING_ENABLED_STDOUT
    logger::error(std::format("Error occured: {}", e.what()));
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, std::format("Error occured: {}", e.what()));
#endif
    }
}

void sv::server::start()
{
#ifdef WEBEAST_SERVER_SYNC
    run_sync();  
#elifdef WEBEAST_SERVER_ASYNC
    auto executor = m_AsioCTX.get_executor();
    boost::asio::co_spawn(executor,
        [this]() -> boost::asio::awaitable<void> { co_await run_async(); },
        boost::asio::detached
    );
    m_AsioCTX.run(); 
#endif
}

sv::server::~server()
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Terminating Server");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Terminating Server");
#endif
}