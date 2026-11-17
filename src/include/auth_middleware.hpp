#ifndef AUTH_MIDDLEWARE_IMPL_H
#define AUTH_MIDDLEWARE_IMPL_H

#include "auth_context.hpp"
#include "jwt_service.hpp"
#include "http.hpp"
#include <utility>
#include <nlohmann/json.hpp>

class auth_middleware
{
public:
    explicit auth_middleware(jwt_service &jwt): _jwt(jwt) {}

    template<typename Request, typename Response, typename Next>
    void operator()(Request &req, Response &resp, Next next)
    {
        auto authorization = req.m_Headers["Authorization"];

        if (authorization.empty() || !authorization.starts_with("Bearer "))
        {
            resp.set_status_code(http::http_status::Unauthorized);
            resp.set_body(nlohmann::json{
                {"error", {
                    {"code", "INVALID_TOKEN"},
                    {"message", "Auhtorization token is missing"}
                }}
            });
            return;
        }

        const auto token = authorization.substr(7);

        auto user_id = _jwt.verify_token(token);

        if (!user_id)
        {
            resp.set_status_code(http::http_status::Unauthorized);
            resp.set_body(nlohmann::json{
                {"error", {
                    {"code", "INVALID_TOKEN"},
                    {"message", "Auhtorization token is invalid or expired"}
                }}
            });
            return;
        }

        req.m_authContext = auth_context{.user_id = static_cast<int32_t>(user_id.value())};

        next();
    }
private:
    jwt_service &_jwt;
};

#endif
