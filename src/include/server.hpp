#ifndef SERVER_IMPL_H
#define SERVER_IMPL_H

#include <string>
#include <boost/asio.hpp>
#include "router.hpp"
#include "tpool.hpp"
#include <memory>

using boost::asio::ip::tcp;

namespace sv
{

class server
{
public:
    server() = delete;
    server(const std::string &addr, int32_t port);
    server(const std::string &addr, int32_t port, rt::router router);
    void include_router(rt::router router);

    // Static
    void get(const std::string &path, rt::callback_handler &&handler);
    void post(const std::string &path, rt::callback_handler &&handler);
    void put(const std::string &path, rt::callback_handler &&handler);
    void _delete(const std::string &path, rt::callback_handler &&handler);
    void head(const std::string &path, rt::callback_handler &&handler);

    // Dynamic
    void get(const std::string &path, rt::dynamic_callback_handler &&handler);
    void post(const std::string &path, rt::dynamic_callback_handler &&handler);
    void put(const std::string &path, rt::dynamic_callback_handler &&handler);
    void _delete(const std::string &path, rt::dynamic_callback_handler &&handler);
    void head(const std::string &path, rt::dynamic_callback_handler &&handler);

    // Json
    void get(const std::string &path, rt::json_callback_handler &&handler);
    void post(const std::string &path, rt::json_callback_handler &&handler);
    void put(const std::string &path, rt::json_callback_handler &&handler);
    void _delete(const std::string &path, rt::json_callback_handler &&handler);
    void head(const std::string &path, rt::json_callback_handler &&handler);

    // Json Dynamic
    void get(const std::string &path, rt::json_dynamic_callback_handler &&handler);
    void post(const std::string &path, rt::json_dynamic_callback_handler &&handler);
    void put(const std::string &path, rt::json_dynamic_callback_handler &&handler);
    void _delete(const std::string &path, rt::json_dynamic_callback_handler &&handler);
    void head(const std::string &path, rt::json_dynamic_callback_handler &&handler);

    void start();
    ~server();
private:
    rt::router m_Router;
    std::string m_Addr;
    int32_t m_Port;
    boost::asio::io_context m_AsioCTX;
    tcp::acceptor m_Acceptor;
    tpool::thread_pool m_Pool;
    void process_connection(std::shared_ptr<tcp::socket> socket);
    boost::asio::awaitable<void> process_connection_async(std::shared_ptr<tcp::socket> socket);
    void run_sync();
    boost::asio::awaitable<void> run_async();
};

};

#endif
