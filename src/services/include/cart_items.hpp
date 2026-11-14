#ifndef CART_ITEMS_SERVICE_IMPL
#define CART_ITEMS_SERVICE_IMPL

#include "../../repositories/include/cart_items.hpp"
#include "../../include/orm/orm.hpp"
#include <optional>


class cart_items_service
{
public:
    cart_items_service(orm::database &db);
    void create(const cart_items &item);
    std::optional<cart_items> get_by_id(int32_t id);
    std::vector<cart_items> get_by_cart_id(int32_t cart_id);
    std::optional<cart_items> get_by_cart_and_product(int32_t cart_id, int32_t product_id);
    void update_quantity(int32_t id, int32_t quantity);
    bool remove(int32_t id);
    bool remove_by_cart_id(int32_t cart_id);
private:
    carts_items_repository c_items_repo;
};

#endif
