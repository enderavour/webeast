#include "include/product.hpp"
#include <limits>

product_service::product_service(orm::database &db): m_repository(db) {}

std::vector<products> product_service::get_all()
{
    return m_repository.get_all();
}

std::optional<products> product_service::get_by_id(int32_t id)
{
    return m_repository.get_by_id(id);
}

validation_result validate_product_schema(const json& body)
{
    validation_result result;

    if (!body.is_object())
    {
        result.add_error("$", "Expected a JSON object");
        return result;
    }

    if (!body.contains("category_id"))
        result.add_error("category_id", "Field is required");
    else if (!body["category_id"].is_number_integer())
        result.add_error("category_id", "Expected an integer");
    else
    {
        const auto value = body["category_id"].get<std::int64_t>();

        if (value <= 0 ||
            value > std::numeric_limits<std::int32_t>::max())
        {
            result.add_error("category_id", "Must be a positive 32-bit integer");
        }
    }

    if (!body.contains("name"))
        result.add_error("name", "Field is required");
    else if (!body["name"].is_string())
        result.add_error("name", "Expected a string");
    else
    {
        const auto& value = body["name"].get_ref<const std::string&>();

        if (value.empty() ||
            value.find_first_not_of(" \t\n\r") == std::string::npos)
        {
            result.add_error("name", "Must not be empty");
        }
        else if (value.size() > 200)
        {
            result.add_error("name", "Must be at most 200 bytes");
        }
    }

    if (body.contains("description") &&
        !body["description"].is_string())
        result.add_error("description", "Expected a string");

    if (!body.contains("price"))
        result.add_error("price", "Field is required");
    else if (!body["price"].is_number_integer())
        result.add_error("price", "Expected an integer");
    else
    {
        const auto value = body["price"].get<std::int64_t>();

        if (value < 0 ||
            value > std::numeric_limits<std::int32_t>::max())
        {
            result.add_error("price", "Must be a non-negative 32-bit integer");
        }
    }

    if (!body.contains("stock"))
        result.add_error("stock", "Field is required");
    else if (!body["stock"].is_number_integer())
        result.add_error("stock", "Expected an integer");
    else
    {
        const auto value = body["stock"].get<std::int64_t>();

        if (value < 0 ||
            value > std::numeric_limits<std::int32_t>::max())
        {
            result.add_error("stock", "Must be a non-negative 32-bit integer");
        }
    }

    if (body.contains("id"))
        result.add_error("id", "Field is not allowed");

    return result;
}
