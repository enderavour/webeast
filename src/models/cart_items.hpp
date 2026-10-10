#ifndef CARTITEMS_MODEL_IMPL_HPP
#define CARTITEMS_MODEL_IMPL_HPP

#include <cstdint>
#include "../include/orm/orm.hpp"

struct cart_items
{
    int32_t id;
    int32_t cart_id;
    int32_t product_id;
    int32_t quantity;
};


template<>
struct orm::orm_model<cart_items> : orm::orm_model_base<cart_items>
{
    static constexpr const char *table = "cart_items";

    static constexpr auto columns = std::make_tuple(
        orm::column("id", &cart_items::id, orm::primary_key),
        orm::column("cart_id", &cart_items::cart_id),
        orm::column("product_id", &cart_items::product_id),
        orm::column("quantity", &cart_items::quantity)
    );

    static inline auto constraints = std::make_tuple(
        orm::foreign_key(
            "carts",
            {"cart_id"},
            {"id"}
        ),

        orm::foreign_key(
            "products",
            {"product_id"},
            {"id"}
        )
    );
};

#endif
