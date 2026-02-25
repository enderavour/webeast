#ifndef ROUTER_IMPL_H
#define ROUTER_IMPL_H

#include <string>
#include <map>
#include <utility>
#include <functional>
#include "http.hpp"

using CallbackHandler = std::function<void(const Request<std::string>&, Response<std::string>&)>;
using RouteCallback = std::pair<HttpMethods, CallbackHandler>;

class Router
{
public:
    Router();
    void register_handler(const std::string &path, HttpMethods method, CallbackHandler &&callback);
    void remove_handler(const std::string &path, HttpMethods method);
    void set_404_handler(CallbackHandler &&callback);
    void set_405_handler(CallbackHandler &&callback);
    CallbackHandler get_handler(const std::string &path, HttpMethods method) const;
    bool dispatch(const Request<std::string> &req, Response<std::string> &res);
private:
    std::map<std::string, std::map<HttpMethods, CallbackHandler>> m_Callbacks;
    CallbackHandler m_notFoundHandler;
    CallbackHandler m_notAllowedHandler;
};

#endif