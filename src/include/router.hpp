#ifndef ROUTER_IMPL_H
#define ROUTER_IMPL_H

#include <string>
#include <map>
#include <utility>
#include <functional>
#include "http.hpp"
#include <boost/regex.hpp>
#include <variant>

using CallbackHandler = std::function<void(const Request<std::string>&, Response<std::string>&)>;
using DynamicCallbackHandler = std::function<void(const Request<std::string>&, Response<std::string>&, boost::smatch&)>;
using RouteCallback = std::pair<HttpMethods, CallbackHandler>;

struct DynamicRoute
{
    boost::regex pattern;
    HttpMethods method;
    DynamicCallbackHandler handler;
};

class Router
{
public:
    Router();
    void register_handler(const std::string &path, HttpMethods method, CallbackHandler &&callback);
    void remove_handler(const std::string &path, HttpMethods method);
    void register_dynamic(const std::string &path, HttpMethods method, DynamicCallbackHandler &&handler);
    void set_404_handler(CallbackHandler &&callback);
    void set_405_handler(CallbackHandler &&callback);
    std::pair<int32_t, std::variant<CallbackHandler, DynamicCallbackHandler>> 
    get_handler(const std::string &path, HttpMethods method, boost::smatch &out_match) const;
    bool dispatch(const Request<std::string> &req, Response<std::string> &res);
private:
    std::map<std::string, std::map<HttpMethods, CallbackHandler>> m_Callbacks;
    std::vector<DynamicRoute> m_DynamicRoutes;
    CallbackHandler m_notFoundHandler;
    CallbackHandler m_notAllowedHandler;
};

#endif