#include "include/jwt_service.hpp"
#include <jwt-cpp/jwt.h>
#include <limits>
#include <optional>
#include <stdexcept>
#include <chrono>

jwt_service::jwt_service(std::string secret): _secret(std::move(secret))
{
    if (_secret.empty())
        throw std::invalid_argument("JWT secret cannot be empty");
}


std::string jwt_service::create_token(int32_t user_id)
{
    using namespace std::chrono;

    const auto now = system_clock::now();

    return jwt::create()
        .set_type("JWT")
        .set_issuer("webeast")
        .set_subject(std::to_string(user_id))
        .set_issued_at(now)
        .set_expires_at(now + hours{1})
        .sign(jwt::algorithm::hs256{_secret});
}

std::optional<int64_t> jwt_service::verify_token(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode(token);

        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{_secret})
            .with_issuer("webeast");

        verifier.verify(decoded);

        const std::string subject = decoded.get_subject();

        std::size_t pos = 0;
        const int64_t value = std::stoll(subject, &pos);

        if (pos != subject.size() || value <= 0 || value > std::numeric_limits<int32_t>::max())
            return std::nullopt;

        return value;
    } catch (const std::exception&)
    {
        return std::nullopt;
    }
}
