#ifndef ROUTER_IMPL_H
#define ROUTER_IMPL_H

#include <string>
#include <map>
#include <utility>
#include <functional>
#include "http.hpp"
#include <boost/regex.hpp>
#include <nlohmann/json.hpp>
#include <variant>

using CallbackHandler = std::function<void(const Request<std::string>&, Response<std::string>&)>;
using DynamicCallbackHandler = std::function<void(const Request<std::string>&, Response<std::string>&, boost::smatch&)>;
using JsonCallbackHandler = std::function<void(const Request<nlohmann::json>&, Response<nlohmann::json>&)>;

struct DynamicRoute
{
    boost::regex pattern;
    HttpMethods method;
    DynamicCallbackHandler handler;
};

struct JsonRoute
{
    std::string path;
    HttpMethods method;
    JsonCallbackHandler handler;
};

class Router
{
public:
    Router();
    void register_handler(const std::string &path, HttpMethods method, CallbackHandler &&callback);
    void remove_handler(const std::string &path, HttpMethods method);
    void register_dynamic(const std::string &path, HttpMethods method, DynamicCallbackHandler &&handler);
    void register_json(const std::string &path, HttpMethods method, JsonCallbackHandler &&handler);
    void set_404_handler(CallbackHandler &&callback);
    void set_405_handler(CallbackHandler &&callback);
    std::pair<int32_t, std::variant<CallbackHandler, DynamicCallbackHandler, JsonCallbackHandler>> 
    get_handler(const std::string &path, HttpMethods method, boost::smatch &out_match) const;
    bool dispatch(const Request<std::string> &req, Response<std::string> &res);
    ~Router();
private:
    std::map<std::string, std::map<HttpMethods, CallbackHandler>> m_Callbacks;
    std::vector<DynamicRoute> m_DynamicRoutes;
    std::vector<JsonRoute> m_JsonRoutes;
    CallbackHandler m_notFoundHandler;
    CallbackHandler m_notAllowedHandler;
};

#endif