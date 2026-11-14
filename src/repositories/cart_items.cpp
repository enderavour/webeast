#include "include/cart_items.hpp"
#include "include/carts.hpp"
#include "include/product.hpp"
#include <tuple>

carts_items_repository::carts_items_repository(orm::database &db): m_db(db) {}

void carts_items_repository::create(const cart_items &item)
{
    m_db.insert<cart_items>(item);
}

std::optional<cart_items> carts_items_repository::get_by_id(int32_t id)
{
    auto condition = orm::where(&cart_items::id, id);
    auto cart_item = m_db.select<cart_items>(condition, id);
    if (cart_item.empty())
        return std::nullopt;

    return cart_item[0];
}

std::vector<cart_items> carts_items_repository::get_by_cart_id(int32_t id)
{
    auto condition = orm::where(&cart_items::id, id);
    return m_db.select<cart_items>(condition, id);
}

std::optional<cart_items> carts_items_repository::get_by_cart_and_product(int32_t cart_id, int32_t product_id)
{
    auto condition_cart = orm::where(&carts::id, cart_id);
    auto condition_product = orm::where(&products::id, product_id);

    auto result = m_db.select<cart_items>(condition_cart, condition_product);
    if (result.empty())
        return std::nullopt;

    return result[0];
}

void carts_items_repository::update_quantity(int32_t id, int32_t quantity)
{
    auto fields = std::make_tuple(orm::set(&cart_items::quantity, quantity));

    auto update_id = orm::where(&cart_items::id, id);

    m_db.update<cart_items>(update_id, fields);
}

bool carts_items_repository::remove(int32_t id)
{
    auto condition = orm::where(&cart_items::id, id);

    return m_db.remove<cart_items>(condition);
}

bool carts_items_repository::remove_by_cart_id(int32_t cart_id)
{
    auto condition = orm::where(&cart_items::cart_id, cart_id);

    return m_db.remove<cart_items>(condition);
}
