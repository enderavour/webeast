#include "include/carts.hpp"
#include <optional>

carts_service::carts_service(orm::database &db): _carts_repo(db) {}

void carts_service::create(const carts &cart)
{
    _carts_repo.create(cart);
}

std::optional<carts> carts_service::get_by_id(int32_t carts_id)
{
    return _carts_repo.get_by_id(carts_id);
}

std::optional<carts> carts_service::get_by_user_id(int32_t user_id)
{
    return _carts_repo.get_by_user_id(user_id);
}

bool carts_service::remove(int32_t id)
{
    return _carts_repo.remove(id);
}

bool carts_service::remove_by_user_id(int32_t id)
{
    return _carts_repo.remove_by_user_id(id);
}
