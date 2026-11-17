#include "include/carts.hpp"

carts_repository::carts_repository(orm::database &db): m_db(db) {}

void carts_repository::create(const carts &cart)
{
    m_db.insert<carts>(cart);
}

std::optional<carts> carts_repository::get_by_id(int32_t id)
{
    auto condition = orm::where(&carts::id, id);
    auto cart = m_db.select<carts>(condition);
    if (cart.empty())
        return std::nullopt;

    return cart[0];
}

std::optional<carts> carts_repository::get_by_user_id(int32_t user_id)
{
    auto condition = orm::where(&carts::user_id, user_id);
    auto cart = m_db.select<carts>(condition);
    if (cart.empty())
        return std::nullopt;

    return cart[0];
}

bool carts_repository::remove(int32_t id)
{
    auto condition = orm::where(&carts::id, id);
    return m_db.remove<carts>(condition);
}

bool carts_repository::remove_by_user_id(int32_t user_id)
{
    auto condition = orm::where(&carts::user_id, user_id);
    return m_db.remove<carts>(condition);
}
