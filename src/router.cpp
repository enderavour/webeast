#include "include/router.hpp"
#include "boost/regex/v5/regex_match.hpp"
#include "include/http.hpp"
#include "include/defaults.hpp"
#include "include/logger.hpp"
#include "include/config.hpp"
#include <boost/algorithm/string/replace.hpp>
#include <iostream>

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

rt::router::router()
{
    LOG_INFO("Created Router, filled with default 404/405 callbacks");
    m_notFoundHandler = defaults::default_404_handler;
    m_notAllowedHandler = defaults::default_405_handler;
}

void rt::router::register_handler(const std::string &path, http::http_method method, rt::callback_handler &&callback)
{
    m_Callbacks[path][method] = std::move(callback);
    LOG_INFO(std::format("Added callback handler at path {}", path));
}

void rt::router::remove_handler(const std::string &path, http::http_method method)
{
    auto entry = m_Callbacks.find(path);
    if (entry == m_Callbacks.end())
    {
        LOG_ERROR(std::format("Could not find any callback handler at path {}", path));
        return;
    }
    auto &method_map = entry->second;
    method_map.erase(method);

    if (method_map.empty())
        m_Callbacks.erase(entry);

    LOG_INFO(std::format("Removed handler at path {}", path));
 }

void rt::router::register_dynamic(const std::string &path, http::http_method method, rt::dynamic_callback_handler &&callback)
{
    std::string regex_path = path;
    boost::replace_all(regex_path, "{", "([^/]+");
    boost::replace_all(regex_path, "}", ")");
    regex_path = "^" + regex_path + "$";
    m_DynamicRoutes.emplace_back(boost::regex(regex_path), method, callback);
    LOG_INFO(std::format("Registered dynamic method at {}", path));
}

void rt::router::register_json(const std::string &path, http::http_method method, rt::json_callback_handler &&handler)
{
    m_JsonRoutes.emplace_back(path, method, handler);

    LOG_INFO(std::format("Registered method with JSON handler at {}", path));
}

void rt::router::register_json_dynamic(const std::string &path, http::http_method method, rt::json_dynamic_callback_handler &&handler)
{
    std::string regex_path = path;
    boost::replace_all(regex_path, "{", "([^/]+");
    boost::replace_all(regex_path, "}", ")");
    regex_path = "^" + regex_path + "$";

    m_JsonDynamicRoutes.emplace_back(boost::regex(regex_path), method, handler);
}

void rt::router::set_404_handler(rt::callback_handler &&callback)
{
    m_notFoundHandler = std::move(callback);
}

void rt::router::set_405_handler(rt::callback_handler &&callback)
{
    m_notAllowedHandler = std::move(callback);
}

bool rt::router::dispatch(const http::request<std::string> &req, http::response_builder<std::string> &res)
{
    auto path_it = m_Callbacks.find(req.m_Path);
    if (path_it == m_Callbacks.end())
    {
        LOG_WARN( std::format("Dispatch: The provided path is not found: {}", req.m_Path));
        m_notFoundHandler(req, res);
        res.set_status_code(http::http_status::NotFound);
        return false;
    }

    auto &method_map = path_it->second;
    auto method_it = method_map.find(req.m_Method);
    if (method_it == method_map.end())
    {
        LOG_WARN( std::format("Dispatch: The provided path is not allowed at path: {}", req.m_Path));
        m_notAllowedHandler(req, res);
        res.set_status_code(http::http_status::MethodNotAllowed);
        return false;
    }

    method_it->second(req, res);
    return true;
}


rt::handler_return_t rt::router::get_handler(
    const std::string& path,
    http::http_method method,
    boost::smatch& out_match
) const
{
    const auto json_method = [&]() -> http::http_method
    {
        switch (method)
        {
            case http::http_method::Get:
                return http::http_method::Json_Get;
            case http::http_method::Post:
                return http::http_method::Json_Post;
            case http::http_method::Put:
                return http::http_method::Json_Put;
            case http::http_method::Delete:
                return http::http_method::Json_Delete;
            case http::http_method::Head:
                return http::http_method::Json_Head;
            default:
                return http::http_method::Json_Unknown;
        }
    }();

    // 1. Static regular routes
    auto callbacks_it = m_Callbacks.find(path);

    if (callbacks_it != m_Callbacks.end())
    {
        const auto& method_map = callbacks_it->second;
        auto method_it = method_map.find(method);

        if (method_it != method_map.end())
            return {0, method_it->second};
    }

    // 2. Static JSON routes
    for (const auto& route : m_JsonRoutes)
    {
        if (route.path == path && route.method == json_method)
            return {2, route.handler};
    }

    // 3. Dynamic regular routes
    for (const auto& route : m_DynamicRoutes)
    {
        if (route.method != method)
            continue;

        boost::smatch match;

        if (boost::regex_match(path, match, route.pattern))
        {
            out_match = match;
            return {1, route.handler};
        }
    }

    // 4. Dynamic JSON routes
    for (const auto& route : m_JsonDynamicRoutes)
    {
        if (route.method != json_method)
            continue;

        boost::smatch match;

        if (boost::regex_match(path, match, route.pattern))
        {
            out_match = match;
            return {3, route.handler};
        }
    }

    // 5. No matching route
    LOG_WARN(
        std::format("Dispatch: The provided path is not found: {}", path)
    );

    return {0, m_notFoundHandler};
}


rt::router::~router()
{
    LOG_INFO("Terminating Router");
}
