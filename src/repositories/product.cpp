#include "include/product.hpp"
#include <optional>

product_repository::product_repository(orm::database &db): m_db(db) {}

std::vector<products> product_repository::get_all()
{
    return m_db.select_all<products>();
}

std::optional<products> product_repository::get_by_id(int32_t id)
{
    auto matching_products = m_db.select<products>("WHERE id = ?", id);
    if (matching_products.empty())
        return std::nullopt;

    return matching_products[0];
}
