#include "include/router.hpp"
#include "include/http.hpp"
#include "include/defs.hpp"
#include "include/logger.hpp"
#include <boost/algorithm/string/replace.hpp>

Router::Router()
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Created Router, filled with default 404/405 callbacks");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Created Router, filled with default 404/405 callbacks");
#endif
    m_notFoundHandler = defaults::default_404_handler;
    m_notAllowedHandler = defaults::default_405_handler;
}

void Router::register_handler(const std::string &path, HttpMethods method, CallbackHandler &&callback)
{
    m_Callbacks[path][method] = std::move(callback);
#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Added callback handler at path {}", path));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Added callback handler at path {}", path));
#endif
}

void Router::remove_handler(const std::string &path, HttpMethods method)
{
    auto entry = m_Callbacks.find(path);
    if (entry == m_Callbacks.end())
    {
#ifdef LOGGING_ENABLED_STDOUT
        logger::error(
            std::format("Could not find any callback handler at path {}", path)
        );
#elifdef LOGGING_ENABLED_FILE
        logger::error(
            defaults::LOG_FILE_HANDLE,
            std::format("Could not find any callback handler at path {}", path)
        );
#endif
        return;
    }

    auto &method_map = entry->second;
    method_map.erase(method);

    if (method_map.empty())
        m_Callbacks.erase(entry);

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Removed handler at path {}", path));
#elifdef LOGGING_ENABLED_FILE 
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Removed handler at path {}", path));
#endif
}

void Router::register_dynamic(const std::string &path, HttpMethods method, DynamicCallbackHandler &&callback)
{
    std::string regex_path = path;
    boost::replace_all(regex_path, "{", "([^/]+");
    boost::replace_all(regex_path, "}", ")");
    regex_path = "^" + regex_path + "$";
    m_DynamicRoutes.emplace_back(boost::regex(regex_path), method, callback);

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Registered dynamic method at {}", path));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, std::format("Registered dynamic method at {}", path));
#endif
}

void Router::register_json(const std::string &path, HttpMethods method, JsonCallbackHandler &&handler)
{
    m_JsonRoutes.emplace_back(path, method, handler);

#ifdef LOGGING_ENABLED_STDOUT
    logger::info(std::format("Registered method with JSON handler at {}", path));
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, 
        std::format("Registered method with JSON handler at {}", path));
#endif

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
#ifdef LOGGING_ENABLED_STDOUT
    logger::warn(std::format("Dispatch: The provided path is not found: {}", req.m_Path));
#elifdef LOGGING_ENABLED_FILE
    logger::warn(defaults::LOG_FILE_HANDLE, std::format("Dispatch: The provided path is not found: {}", req.m_Path));
#endif
        m_notFoundHandler(req, res);
        res.set_status_code(HttpStatus::NotFound);
        return false;
    }

    auto &method_map = path_it->second;
    auto method_it = method_map.find(req.m_Method);
    if (method_it == method_map.end())
    {
#ifdef LOGGING_ENABLED_STDOUT
    logger::warn(std::format("Dispatch: The provided path is not allowed at path: {}", req.m_Path));
#elifdef LOGGING_ENABLED_FILE
    logger::warn(defaults::LOG_FILE_HANDLE, std::format("Dispatch: The provided path is not found: {}", req.m_Path));
#endif
        m_notAllowedHandler(req, res);
        res.set_status_code(HttpStatus::MethodNotAllowed);
        return false;
    }

    method_it->second(req, res);
    return true;
}


std::pair<int32_t, std::variant<CallbackHandler, DynamicCallbackHandler, JsonCallbackHandler>> 
Router::get_handler(const std::string &path, HttpMethods method, boost::smatch &out_match) const
{
    if ((int32_t)method >= 5)
    {
        for (const auto &jroute: m_JsonRoutes)
        {
            if (jroute.path == path && jroute.method == method)
                return {2, jroute.handler};
        }      
    }

    auto callbacks_it = m_Callbacks.find(path);
    if (callbacks_it != m_Callbacks.end())
    {
        const auto &method_map = callbacks_it->second;

        auto method_it = method_map.find(method);
        if (method_it != method_map.end())
            return {0, method_it->second};
        
#ifdef LOGGING_ENABLED_STDOUT
    logger::warn(std::format(
        "Dispatch: The provided path is not allowed at path: {}", path
    ));
#elifdef LOGGING_ENABLED_FILE
    logger::warn(
        defaults::LOG_FILE_HANDLE,
        std::format("Dispatch: The provided path is not allowed at path: {}", path)
    );
#endif
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
#ifdef LOGGING_ENABLED_STDOUT
    logger::warn(std::format("Dispatch: The provided path is not found: {}", path));
#elifdef LOGGING_ENABLED_FILE
    logger::warn(
        defaults::LOG_FILE_HANDLE,
        std::format("Dispatch: The provided path is not found: {}", path)
    );
#endif
    return {0, m_notFoundHandler};
}

Router::~Router()
{
#ifdef LOGGING_ENABLED_STDOUT
    logger::info("Terminating Router");
#elifdef LOGGING_ENABLED_FILE
    logger::info(defaults::LOG_FILE_HANDLE, "Terminating Router");
#endif
}