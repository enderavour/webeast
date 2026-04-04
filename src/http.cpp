#include "include/http.hpp"
#include <sstream>
#include <unordered_map>
#include <format>
#include <optional>

namespace urls = boost::urls;

template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
http::response_builder<T>::response_builder() = default;

template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
http::response_builder<T>::response_builder(
    http::http_status statusCode, T &&body, 
    const std::map<std::string, std::string> &headers):
m_StatusCode(statusCode), m_Body(body), m_Headers(headers) {}

template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
void http::response_builder<T>::set_status_code(http::http_status code)
{
    m_StatusCode = code;
}


template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
void http::response_builder<T>::set_header(const std::string &key, const std::string &value)
{
    m_Headers[key] = value;
}


template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
void http::response_builder<T>::set_body(const T &body) 
{
    m_Body = body;
}


template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
http::http_status http::response_builder<T>::get_status_code() const
{
    return m_StatusCode;
}


template<class T>
requires std::is_convertible_v<T, std::string> ||  
std::is_same_v<std::decay_t<T>, nlohmann::json>
std::map<std::string, std::string> http::response_builder<T>::get_headers() const
{
    return m_Headers;
}


template<class T>
requires std::is_convertible_v<T, std::string> || 
std::is_same_v<std::decay_t<T>, nlohmann::json>
std::decay_t<T> http::response_builder<T>::get_body() const
{
    return m_Body;
}

template class http::response_builder<std::string>;
template class http::response_builder<std::string&>;
template class http::response_builder<const std::string&>;
template class http::response_builder<std::string&&>;
template class http::response_builder<nlohmann::json>;
template class http::response_builder<nlohmann::json&&>;

// Helper function for converting string method to HtpMethods enumeration
http::http_method convert_method(const std::string &method)
{
    const std::unordered_map<std::string, http::http_method> methods_map = {
        {"GET", http::http_method::Get},
        {"POST", http::http_method::Post},
        {"PUT", http::http_method::Put},
        {"DELETE", http::http_method::Delete}
    };

    auto http_method = methods_map.find(method);
    if (http_method == methods_map.end())
        return http::http_method::Unknown;
    
    return http_method->second;
}

http::request<std::string> http::deserialize_request(const std::string &source)
{
    // For parsing Method, Path and HTTP version (first line of HTTP request)
    std::istringstream stream(source);
    std::string method, path, http;
    stream >> method >> path >> http;

    std::string _dummy;
    std::getline(stream, _dummy);

    http::request<std::string> rq{};
    
    rq.m_Method = convert_method(method);
    rq.m_Path = path;
    rq.m_HttpVersion = http;
    
    std::string header;
    while (std::getline(stream, header))
    {
        if (!header.empty() && header.back() == '\r')
            header.pop_back();

        if (header.empty())
            break;

        std::string key, value;
        auto pos = header.find(':');
        if (pos != std::string::npos)
        {
            key = header.substr(0, pos);
            value = header.substr(pos + 1);

            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" ") + 1);

            rq.m_Headers[key] = value;
        }
    }
    return rq;
}

// Helper function to convert Method enum to string
std::optional<std::string> status_to_string(http::http_status status)
{
    const std::unordered_map<http::http_status, std::string> method_map = {
        {http::http_status::Ok, "OK"},
        {http::http_status::MethodNotAllowed, "Method Not Allowed"},
        {http::http_status::NotFound, "Not Found"},
        {http::http_status::BadRequest, "Bad Request"},
        {http::http_status::Created, "Created"}
    };

    auto status_str = method_map.find(status);
    if (status_str == method_map.end())
        return {};
    
    return status_str->second;
}

std::string http::serialize_response(const http::response_builder<std::string> &resp)
{
    std::string payload;
    payload += std::format(
        "HTTP/1.1 {} {}\r\n",
        static_cast<int32_t>(resp.get_status_code()),
        status_to_string(resp.get_status_code()).value_or("Unknown")
    );

    for (const auto &[key, value]: resp.get_headers())
    {
        payload += std::format(
            "{}: {}\r\n",
            key, value
        );
    }

    payload += "\r\n";
    payload += resp.get_body();

    return payload;
}

std::string http::serialize_response(const http::response_builder<nlohmann::json> &resp)
{
    std::string payload;
    payload += std::format(
        "HTTP/1.1 {} {}\r\n",
        static_cast<int32_t>(resp.get_status_code()),
        status_to_string(resp.get_status_code()).value_or("Unknown")
    );

    for (const auto &[key, value]: resp.get_headers())
    {
        payload += std::format(
            "{}: {}\r\n",
            key, value
        );
    }

    payload += "\r\n";
    payload += resp.get_body().dump();

    return payload;
}

http::query_params http::parse_body_params(std::string_view url)
{
    http::query_params params;
    urls::params_view uview(url);
    for (auto const &p: uview)
    params[p.key] = p.value;

    return params;
}
