#include "include/router.hpp"
#include "include/http.hpp"

Router::Router()
{
    m_notFoundHandler = [](const Request<std::string>&, Response<std::string> &res)
    {
        res.set_status_code(HttpStatus::NotFound);
        res.set_body("404 Not Found");
    };

    m_notAllowedHandler = [](const Request<std::string>&, Response<std::string> &res)
    {
        res.set_status_code(HttpStatus::MethodNotAllowed);
        res.set_body("405 Not Allowed");
    };
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

CallbackHandler Router::get_handler(const std::string &path, HttpMethods method) const
{
    auto callbacks_it = m_Callbacks.find(path);
    if (callbacks_it == m_Callbacks.end())
        return m_notFoundHandler;

    const auto &method_map = callbacks_it->second;
    auto method_it = method_map.find(method);
    if (method_it == method_map.end())
        return m_notAllowedHandler;

    return method_it->second;
}