#include "include/http.hpp"
#include <sstream>
#include <unordered_map>
#include <format>
#include <optional>

template<class T>
requires std::is_convertible_v<T, std::string>
Response<T>::Response() = default;

template<class T>
requires std::is_convertible_v<T, std::string>
Response<T>::Response(HttpStatus statusCode, T &&body, const std::map<std::string, std::string> &headers):
m_StatusCode(statusCode), m_Body(body), m_Headers(headers) {}

template<class T>
requires std::is_convertible_v<T, std::string>
void Response<T>::set_status_code(HttpStatus code)
{
    m_StatusCode = code;
}


template<class T>
requires std::is_convertible_v<T, std::string>
void Response<T>::set_header(const std::string &key, const std::string &value)
{
    m_Headers[key] = value;
}


template<class T>
requires std::is_convertible_v<T, std::string>
void Response<T>::set_body(T &&body) 
{
    m_Body = body;
}


template<class T>
requires std::is_convertible_v<T, std::string>
HttpStatus Response<T>::get_status_code() const
{
    return m_StatusCode;
}


template<class T>
requires std::is_convertible_v<T, std::string>
std::map<std::string, std::string> Response<T>::get_headers() const
{
    return m_Headers;
}


template<class T>
requires std::is_convertible_v<T, std::string>
std::decay_t<T> Response<T>::get_body() const
{
    return m_Body;
}

template class Response<std::string>;
template class Response<std::string&>;
template class Response<const std::string&>;
template class Response<std::string&&>;

// Helper function for converting string method to HtpMethods enumeration
HttpMethods convert_method(const std::string &method)
{
    const std::unordered_map<std::string, HttpMethods> methods_map = {
        {"GET", HttpMethods::GET},
        {"POST", HttpMethods::POST}
    };

    auto http_method = methods_map.find(method);
    if (http_method == methods_map.end())
        return HttpMethods::UNKNOWN;
    
    return http_method->second;
}

Request<std::string> deserialize_request(const std::string &source)
{
    // For parsing Method, Path and HTTP version (first line of HTTP request)
    std::istringstream stream(source);
    std::string method, path, http;
    stream >> method >> path >> http;

    Request<std::string> rq{};
    
    HttpMethods converted_method = convert_method(method);
    rq.m_Method = converted_method;
    rq.m_Path = path;
    rq.m_HttpVersion = http;
    
    std::string header;
    while (std::getline(stream, header) && header != "\r")
    {
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
std::optional<std::string> status_to_string(HttpStatus status)
{
    const std::unordered_map<HttpStatus, std::string> method_map = {
        {HttpStatus::OK, "OK"},
        {HttpStatus::MethodNotAllowed, "Method Not Allowed"},
        {HttpStatus::NotFound, "Not Found"},
        {HttpStatus::BadRequest, "Bad Request"},
        {HttpStatus::Created, "Created"}
    };

    auto status_str = method_map.find(status);
    if (status_str == method_map.end())
        return {};
    
    return status_str->second;
}

std::string serialize_response(const Response<std::string> &resp)
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
