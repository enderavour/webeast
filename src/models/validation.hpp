#ifndef MODEL_VALIDATION_IMPL
#define MODEL_VALIDATION_IMPL

#include <nlohmann/json.hpp>
#include <cstdint>
#include <cstring>
#include <vector>

using json = nlohmann::json;

struct validation_error
{
    std::string field;
    std::string message;
};

struct validation_result
{
    std::vector<validation_error> errors;

    [[nodiscard]]
    bool ok() const noexcept;

    void add_error(const std::string &field, const std::string &message);

    [[nodiscard]]
    json to_json() const;
};

#endif
