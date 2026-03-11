#ifndef HTTP_IMPL_H
#define HTTP_IMPL_H

#include <type_traits>
#include <string>
#include <map>
#include <unordered_map>
#include <boost/url.hpp>

using QueryParams = std::unordered_map<std::string, std::string>;

enum class HttpMethods 
{
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
};

enum class HttpStatus
{
    OK = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    MethodNotAllowed = 405
};

template<class T>
requires std::is_convertible_v<T, std::string>
struct Request
{
    HttpMethods m_Method;
    std::string m_Path;
    std::string m_HttpVersion;

    std::map<std::string, std::string> m_Headers;
    std::map<std::string, std::string> m_Params;

    T m_Body;
};

template<class T>
requires std::is_convertible_v<T, std::string>
class Response
{
public:
    using BodyType = std::decay_t<T>;

    Response();
    Response(HttpStatus statusCode, T &&body, const std::map<std::string, std::string> &headers);
    
    void set_status_code(HttpStatus code);
    void set_header(const std::string &key, const std::string &value);
    void set_body(const T &body);

    HttpStatus get_status_code() const;
    std::map<std::string, std::string> get_headers() const;
    BodyType get_body() const;

private:
    HttpStatus m_StatusCode;
    BodyType m_Body;
    std::map<std::string, std::string> m_Headers;
};

Request<std::string> deserialize_request(const std::string &source);
std::string serialize_response(const Response<std::string> &resp);

#define dynamic_get_match(match, index) match[index].str()

QueryParams parse_body_params(std::string_view url);

#endif