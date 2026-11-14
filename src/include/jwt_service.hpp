#ifndef JWT_SERVICE_IMPL
#define JWT_SERVICE_IMPL

#include <optional>
#include <string>

class jwt_service
{
public:
    explicit jwt_service(std::string secret);

    std::string create_token(int32_t user_id);
    std::optional<int64_t> verify_token(const std::string &token);
private:
    std::string _secret;
};

#endif
