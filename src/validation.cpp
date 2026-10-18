#include "models/validation.hpp"

using namespace nlohmann;

bool validation_result::ok() const noexcept
{
    return errors.empty();
}

void validation_result::add_error(const std::string &field, const std::string &message)
{
    errors.emplace_back(field, message);
}

json validation_result::to_json() const
{
    basic_json<> obj = json::array();

    for (const auto &[field, message]: errors)
        obj.push_back({
            {"field", field},
            {"message", message}
        });

    return obj;
}
