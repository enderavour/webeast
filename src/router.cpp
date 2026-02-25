#include "include/router.hpp"
#include "include/http.hpp"

Router::Router() = default;

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
    auto callbacks_set = callbacks_it->second;

    auto callback = callbacks_set.find(method);
    return callback->second;
}