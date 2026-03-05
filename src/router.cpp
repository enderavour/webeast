#include "include/router.hpp"
#include "include/http.hpp"
#include "include/defs.hpp"
#include <boost/algorithm/string/replace.hpp>

Router::Router()
{
    m_notFoundHandler = defaults::default_404_handler;
    m_notAllowedHandler = defaults::default_405_handler;
}

void Router::register_handler(const std::string &path, HttpMethods method, CallbackHandler &&callback)
{
    m_Callbacks[path][method] = std::move(callback);
}

void Router::remove_handler(const std::string &path, HttpMethods method)
{
    auto entry = m_Callbacks.find(path);
    if (entry == m_Callbacks.end())
        return;

    auto &method_map = entry->second;
    method_map.erase(method);

    if (method_map.empty())
        m_Callbacks.erase(entry);
}

void Router::register_dynamic(const std::string &path, HttpMethods method, DynamicCallbackHandler &&callback)
{
    std::string regex_path = path;
    boost::replace_all(regex_path, "{", "([^/]+");
    boost::replace_all(regex_path, "}", ")");
    regex_path = "^" + regex_path + "$";
    m_DynamicRoutes.push_back({boost::regex(regex_path), method, callback});
}

void Router::set_404_handler(CallbackHandler &&callback)
{
    m_notFoundHandler = std::move(callback);
}

void Router::set_405_handler(CallbackHandler &&callback)
{
    m_notAllowedHandler = std::move(callback);
}

bool Router::dispatch(const Request<std::string> &req, Response<std::string> &res)
{
    auto path_it = m_Callbacks.find(req.m_Path);
    if (path_it == m_Callbacks.end())
    {
        m_notFoundHandler(req, res);
        res.set_status_code(HttpStatus::NotFound);
        return false;
    }

    auto &method_map = path_it->second;
    auto method_it = method_map.find(req.m_Method);
    if (method_it == method_map.end())
    {
        m_notAllowedHandler(req, res);
        res.set_status_code(HttpStatus::MethodNotAllowed);
        return false;
    }

    method_it->second(req, res);
    return true;
}

std::pair<int32_t, std::variant<CallbackHandler, DynamicCallbackHandler>> 
Router::get_handler(const std::string &path, HttpMethods method, boost::smatch &out_match) const
{
    auto callbacks_it = m_Callbacks.find(path);
    if (callbacks_it != m_Callbacks.end())
    {
        const auto &method_map = callbacks_it->second;

        auto method_it = method_map.find(method);
        if (method_it != method_map.end())
            return {0, method_it->second};
        
        return {0, m_notAllowedHandler};
    }
        
    for (const auto &route: m_DynamicRoutes)
    {
        if (route.method != method)
            continue;

        boost::smatch match;
        if (boost::regex_match(path, match, route.pattern))
        {
            out_match = match;
            return {1, route.handler};
        }
        out_match = match;
    }
    return {0, m_notFoundHandler};
}