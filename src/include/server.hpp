#ifndef SERVER_IMPL_H
#define SERVER_IMPL_H

#include <string>
#include <boost/asio.hpp>
#include "router.hpp"
#include "tpool.hpp"
#include <memory>

using boost::asio::ip::tcp;

class ServerInstance
{
public:
    ServerInstance() = delete;
    ServerInstance(const std::string &addr, int32_t port);
    ServerInstance(const std::string &addr, int32_t port, Router router);
    void include_router(Router router);
    // Static
    void get(const std::string &path, CallbackHandler &&handler);
    void post(const std::string &path, CallbackHandler &&handler);
    void put(const std::string &path, CallbackHandler &&handler);
    void _delete(const std::string &path, CallbackHandler &&handler);

    // Dynamic
    void get(const std::string &path, DynamicCallbackHandler &&handler);
    void post(const std::string &path, DynamicCallbackHandler &&handler);
    void put(const std::string &path, DynamicCallbackHandler &&handler);
    void _delete(const std::string &path, DynamicCallbackHandler &&handler);

    // Json
    void get(const std::string &path, JsonCallbackHandler&&handler);
    void post(const std::string &path, JsonCallbackHandler &&handler);
    void put(const std::string &path, JsonCallbackHandler &&handler);
    void _delete(const std::string &path, JsonCallbackHandler &&handler);

    // Json Dynamic
    void get(const std::string &path, JsonDynamicCallbackHandler &&handler);
    void post(const std::string &path, JsonDynamicCallbackHandler &&handler);
    void put(const std::string &path, JsonDynamicCallbackHandler &&handler);
    void _delete(const std::string &path, JsonDynamicCallbackHandler &&handler);

    void start();
    ~ServerInstance();
private:
    Router m_Router;
    std::string m_Addr;
    int32_t m_Port;
    boost::asio::io_context m_AsioCTX;
    tcp::acceptor m_Acceptor;
    ThreadPool m_Pool;
    void process_connection(std::shared_ptr<tcp::socket> socket);
    boost::asio::awaitable<void> process_connection_async(std::shared_ptr<tcp::socket> socket);
    void run_sync();
    boost::asio::awaitable<void> run_async();
};

#endif