#include "include/product.hpp"
#include <optional>

product_repository::product_repository(orm::database &db): m_db(db) {}

std::vector<products> product_repository::select_all()
{
    return m_db.select_all<products>();
}

std::optional<products> product_repository::select_by_id(int32_t id)
{
    auto matching_products = m_db.select<products>("WHERE id = ?", id);
    if (matching_products.empty())
        return std::nullopt;

    return matching_products[0];
}

bool product_repository::remove(int32_t id)
{
    auto condition = orm::where(&products::id, id);

    return m_db.remove<products>(condition);
}

void product_repository::update(int32_t id, const products &product)
{
    auto fields = orm::set(
        orm::set_field(&products::category_id, product.category_id),
        orm::set_field(&products::name, product.name),
        orm::set_field(&products::description, product.description),
        orm::set_field(&products::price, product.price),
        orm::set_field(&products::stock, product.stock)
    );

    auto condition = orm::where(&products::id, id);

    m_db.update<products>(fields, condition);
}

void product_repository::create(const products &product)
{
    m_db.insert<products>(product);
}
