#ifndef HTTP_IMPL_H
#define HTTP_IMPL_H

#include <type_traits>
#include <string>
#include <map>
#include <unordered_map>
#include <boost/url.hpp>
#include <nlohmann/json.hpp>

namespace http
{

using query_params = std::unordered_map<std::string, std::string>;

enum class http_method: int32_t
{
    Get,
    Post,
    Put,
    Delete,
    Unknown,
    Head,
    // Versions for returning JSON version of method
    Json_Get,
    Json_Post,
    Json_Put,
    Json_Delete,
    Json_Unknown,
    Json_Head
};

enum class http_status
{
    Ok = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    MethodNotAllowed = 405
};

template<class T>
requires std::is_convertible_v<T, std::string> ||
std::is_same_v<std::decay_t<T>, nlohmann::json>
struct request
{
    http_method m_Method;
    std::string m_Path;
    std::string m_HttpVersion;

    std::map<std::string, std::string> m_Headers;
    std::map<std::string, std::string> m_Params;

    T m_Body;
};

template<class T>
requires std::is_convertible_v<T, std::string> ||
std::is_same_v<std::decay_t<T>, nlohmann::json>
class response_builder
{
public:
    using body_type = std::decay_t<T>;

    response_builder();
    response_builder(http_status statusCode, T &&body, const std::map<std::string, std::string> &headers);

    void set_status_code(http_status code);
    void set_header(const std::string &key, const std::string &value);
    void set_body(const T &body);

    http_status get_status_code() const;
    std::map<std::string, std::string> get_headers() const;
    body_type get_body() const;

private:
    http_status m_StatusCode;
    body_type m_Body;
    std::map<std::string, std::string> m_Headers;
};

request<std::string> deserialize_request(const std::string &source);
std::string serialize_response(const response_builder<std::string> &resp);
std::string serialize_response(const response_builder<nlohmann::json> &resp);

#define dynamic_get_match(match, index) match[index].str()

query_params parse_body_params(std::string_view url);

};

#endif
