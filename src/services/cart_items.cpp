#include "include/cart_items.hpp"

cart_items_service::cart_items_service(orm::database &db): c_items_repo(db) {}

void cart_items_service::create(const cart_items &item)
{
    c_items_repo.create(item);
}

std::optional<cart_items> cart_items_service::get_by_id(int32_t id)
{
    return c_items_repo.get_by_id(id);
}


std::vector<cart_items> cart_items_service::get_by_cart_id(int32_t cart_id)
{
    return c_items_repo.get_by_cart_id(cart_id);
}


std::optional<cart_items> cart_items_service::get_by_cart_and_product(int32_t cart_id, int32_t product_id)
{
    return c_items_repo.get_by_cart_and_product(cart_id, product_id);
}

void cart_items_service::update_quantity(int32_t id, int32_t quantity)
{
    c_items_repo.update_quantity(id, quantity);
}

bool cart_items_service::remove(int32_t id)
{
    c_items_repo.remove(id);
}

bool cart_items_service::remove_by_cart_id(int32_t cart_id)
{
    c_items_repo.remove_by_cart_id(cart_id);
}
