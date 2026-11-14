#ifndef CARTS_ITEMS_REPOSITORY_IMPL
#define CARTS_ITEMS_REPOSITORY_IMPL

#include "../../models/cart_items.hpp"
#include <optional>
#include <vector>

class carts_items_repository
{
public:
    carts_items_repository(orm::database &db);
    void create(const cart_items &item);
    std::optional<cart_items> get_by_id(int32_t id);
    std::vector<cart_items> get_by_cart_id(int32_t cart_id);
    std::optional<cart_items> get_by_cart_and_product(int32_t cart_id, int32_t product_id);
    void update_quantity(int32_t id, int32_t quantity);
    bool remove(int32_t id);
    bool remove_by_cart_id(int32_t cart_id);
public:
    orm::database &m_db;
};


#endif
