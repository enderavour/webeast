#ifndef SERVER_IMPL_H
#define SERVER_IMPL_H

#include <string>
#include <boost/asio.hpp>
#include "router.hpp"
#include "tpool.hpp"
#include <memory>

static constexpr int32_t CLIENTS_MAX_CAPACITY = 25;

using boost::asio::ip::tcp;

class ServerInstance
{
public:
    ServerInstance() = delete;
    ServerInstance(const std::string &addr, int32_t port);
    ServerInstance(const std::string &addr, int32_t port, Router router);
    void include_router(Router router);
    void get(const std::string &path, CallbackHandler &&handler);
    void post(const std::string &path, CallbackHandler &&handler);
    void start();
private:
    Router m_Router;
    std::string m_Addr;
    int32_t m_Port;
    boost::asio::io_context m_AsioCTX;
    tcp::acceptor m_Acceptor;
    ThreadPool m_Pool;
    void process_connection(std::shared_ptr<tcp::socket> socket);
};

#endif