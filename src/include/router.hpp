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

namespace rt
{

using callback_handler = std::function<void(const http::request<std::string>&, http::response_builder<std::string>&)>;
using dynamic_callback_handler = 
std::function<void(const http::request<std::string>&, http::response_builder<std::string>&, boost::smatch&)>;
using json_callback_handler = 
std::function<void(const http::request<nlohmann::json>&, http::response_builder<nlohmann::json>&)>;
using json_dynamic_callback_handler =
 std::function<void(const http::request<nlohmann::json>&, http::response_builder<nlohmann::json>&, boost::smatch&)>;

struct dynamic_route
{
    boost::regex pattern;
    http::http_method method;
    dynamic_callback_handler handler;
};

struct json_route
{
    std::string path;
    http::http_method method;
    json_callback_handler handler;
};

struct json_dynamic_route
{
    boost::regex pattern;
    http::http_method method;
    json_dynamic_callback_handler handler;
};

class router
{
public:
    router();
    void register_handler(const std::string &path, http::http_method method, callback_handler &&callback);
    void remove_handler(const std::string &path, http::http_method method);
    void register_dynamic(const std::string &path, http::http_method method, dynamic_callback_handler &&handler);
    void register_json(const std::string &path, http::http_method method, json_callback_handler &&handler);
    void register_json_dynamic(const std::string &path, http::http_method method, json_dynamic_callback_handler &&handler);
    void set_404_handler(callback_handler &&callback);
    void set_405_handler(callback_handler &&callback);
    std::pair<int32_t, 
    std::variant<
    callback_handler, dynamic_callback_handler, json_callback_handler, 
    json_dynamic_callback_handler>> 
    get_handler(const std::string &path, http::http_method method, boost::smatch &out_match) const;
    bool dispatch(const http::request<std::string> &req, http::response_builder<std::string> &res);
    ~router();
private:
    std::map<std::string, std::map<http::http_method, callback_handler>> m_Callbacks;
    std::vector<dynamic_route> m_DynamicRoutes;
    std::vector<json_route> m_JsonRoutes;
    std::vector<json_dynamic_route> m_JsonDynamicRoutes;
    callback_handler m_notFoundHandler;
    callback_handler m_notAllowedHandler;
};

};

#endif