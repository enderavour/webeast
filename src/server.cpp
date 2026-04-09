#include "include/server.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>
#include "include/defs.hpp"
#include "include/http.hpp"
#include "include/logger.hpp"
#include "include/config.hpp"
#include <print>
#include <format>
#include <nlohmann/json.hpp>

using boost::asio::ip::make_address;
using json = nlohmann::json;

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

sv::server::server(const std::string &addr, int32_t port):
m_Addr(addr), m_Port(port), m_AsioCTX(1), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(CONFIG_OPTS.clients_capacity)
{
    LOG_INFO(CONFIG_OPTS, std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, CONFIG_OPTS.clients_capacity
    ));
}

sv::server::server(const std::string &addr, int32_t port, rt::router router):
m_Router(router), m_Addr(addr), m_Port(port), m_Acceptor(m_AsioCTX, tcp::endpoint(
    make_address(addr), port)), m_Pool(CONFIG_OPTS.clients_capacity)
{
    LOG_INFO(CONFIG_OPTS, std::format(
        "Created server with at address {} and port {}. Max connections: {}",
        addr, port, CONFIG_OPTS.clients_capacity
    ));
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

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering GET request at path: {}", path
    ));
}

void sv::server::post(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Post,
        std::forward<rt::callback_handler>(handler)
    );
    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering POST request at path: {}", path
    ));
}

void sv::server::put(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Put,
        std::forward<rt::callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering PUT request at path: {}", path
    ));
}

void sv::server::_delete(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Delete,
        std::forward<rt::callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering DELETE request at path: {}", path
    ));
}

void sv::server::head(const std::string &path, rt::callback_handler &&handler)
{
    m_Router.register_handler(
        path, http::http_method::Head,
        std::forward<rt::callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering HEAD request at path: {}", path
    ));
}


// Dynamic
void sv::server::get(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Get,
        std::forward<rt::dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering dynamic GET request at path: {}", path
    ));
}

void sv::server::post(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Post,
        std::forward<rt::dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering dynamic POST request at path: {}", path
    ));
}

void sv::server::put(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Put,
        std::forward<rt::dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering dynamic PUT request at path: {}", path
    ));
}

void sv::server::_delete(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Delete,
        std::forward<rt::dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering dynamic DELETE request at path: {}", path
    ));
}

void sv::server::head(const std::string &path, rt::dynamic_callback_handler &&handler)
{
    m_Router.register_dynamic(
        path, http::http_method::Head,
        std::forward<rt::dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering dynamic HEAD request at path: {}", path
    ));
}


// Json
void sv::server::get(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(
        path, http::http_method::Json_Get,
        std::forward<rt::json_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON GET request at path: {}", path
    ));
}

void sv::server::post(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(
        path, http::http_method::Json_Post,
        std::forward<rt::json_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON POST request at path: {}", path
    ));
}

void sv::server::put(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(
        path, http::http_method::Json_Put,
        std::forward<rt::json_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON PUT request at path: {}", path
    ));
}

void sv::server::_delete(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(
        path, http::http_method::Json_Delete,
        std::forward<rt::json_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON DELETE request at path: {}", path
    ));
}

void sv::server::head(const std::string &path, rt::json_callback_handler &&handler)
{
    m_Router.register_json(
        path, http::http_method::Json_Head,
        std::forward<rt::json_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON HEAD request at path: {}", path
    ));
}

// Json Dynamic
void sv::server::get(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Get,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON Dynamic GET request at path: {}", path
    ));
}

void sv::server::post(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Post,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON Dynamic POST request at path: {}", path
    ));
}

void sv::server::put(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Put,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON Dynamic PUT request at path: {}", path
    ));
}

void sv::server::_delete(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Delete,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON Dynamic DELETE request at path: {}", path
    ));
}

void sv::server::head(const std::string &path, rt::json_dynamic_callback_handler &&handler)
{
    m_Router.register_json_dynamic(
        path, http::http_method::Json_Head,
        std::forward<rt::json_dynamic_callback_handler>(handler)
    );

    LOG_INFO(CONFIG_OPTS, std::format(
        "Registering JSON Dynamic HEAD request at path: {}", path
    ));
}

void sv::server::run_sync()
{
    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(m_AsioCTX);
        m_Acceptor.accept(*socket);

        LOG_INFO(CONFIG_OPTS, "Accepted new client");

        if (m_Pool.active_tasks_count() + 1 >= defaults::CLIENTS_MAX_CAPACITY)
        {
            LOG_ERROR(CONFIG_OPTS, "Thread pool capacity exceded. Closing connection.");
            socket->close();
        }

        m_Pool.add_task([this, socket = std::move(socket)]() {
            process_connection(std::move(socket));
        });
        LOG_INFO(CONFIG_OPTS, "Adding processing task to thread pool");
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
            LOG_INFO(CONFIG_OPTS, "Accepted new client");
            boost::asio::co_spawn(executor, [this, sock = std::move(socket)]() mutable -> boost::asio::awaitable<void>
            {
                co_await process_connection_async(sock);
            }, boost::asio::detached);
            LOG_INFO(CONFIG_OPTS, "Started coroutine of processing task");
        }
        catch (const std::exception &e)
        {
            LOG_INFO(CONFIG_OPTS, std::format("Async accept error: {}", e.what()));
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
                LOG_ERROR(CONFIG_OPTS, "Could not parse invalid HTTP request.");
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
                    case http::http_method::Head:
                        request.m_Method = http::http_method::Json_Head;
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
                    LOG_INFO(CONFIG_OPTS, "Client resetted the connection");
                else
                {
                    LOG_ERROR(CONFIG_OPTS, std::format(
                        "Connection error occured: {}", ec.what()
                    ));
                }
            }

            auto it_conn = request.m_Headers.find("Connection");
            if (it_conn != request.m_Headers.end() && it_conn->second == "close")
            {
                LOG_INFO(CONFIG_OPTS, "Client closed the connection");
                break;
            }
        }
        socket->close();
    }
    catch (const std::exception &e)
    {
        LOG_INFO(CONFIG_OPTS, std::format("Error occured: {}", e.what()));
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
                LOG_ERROR(CONFIG_OPTS, "Could not parse invalid HTTP request.");
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
                    case http::http_method::Head:
                        request.m_Method = http::http_method::Json_Head;
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
                    LOG_INFO(CONFIG_OPTS, "Client resetted the connection");
                }
                else
                {
                    LOG_ERROR(CONFIG_OPTS, std::format(
                        "Connection error occured: {}", ec.what()
                    ));
                }
            }

            auto it_conn = request.m_Headers.find("Connection");
            if (it_conn != request.m_Headers.end() && it_conn->second == "close")
            {
                LOG_INFO(CONFIG_OPTS, "Client closed the connection");
                break;
            }
        }
        socket->close();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR(CONFIG_OPTS, std::format("Error occured: {}", e.what()));
    }
}

void sv::server::start()
{
    LOG_INFO(CONFIG_OPTS, "Server is running at 127.0.0.1:8080");

    if (CONFIG_OPTS.server == conf::server_type::Sync)
    {
        run_sync();
    }
    else if (CONFIG_OPTS.server == conf::server_type::Async)
    {
        auto executor = m_AsioCTX.get_executor();
        boost::asio::co_spawn(executor,
            [this]() -> boost::asio::awaitable<void> { co_await run_async(); },
            boost::asio::detached
        );
        m_AsioCTX.run();
    }
}

sv::server::~server()
{
    LOG_INFO(CONFIG_OPTS, "Terminating Server");
}
