#ifndef ORDERITEMS_MODEL_IMPL_HPP
#define ORDERITEMS_MODEL_IMPL_HPP

#include <cstdint>
#include <tuple>
#include "../include/orm/orm.hpp"

struct order_items
{
    int32_t id;
    int32_t order_id;
    int32_t product_id;
    int32_t quantity;
    int32_t price;
};

template<>
struct orm::orm_model<order_items> : orm::orm_model_base<order_items>
{
    static constexpr const char *table = "order_items";

    static constexpr auto columns = std::make_tuple(
        orm::column("id", &order_items::id, orm::primary_key),
        orm::column("order_id", &order_items::order_id),
        orm::column("product_id", &order_items::product_id),
        orm::column("quantity", &order_items::quantity),
        orm::column("price", &order_items::price)
    );

    static inline auto constraints = std::make_tuple(
        orm::foreign_key(
            "orders",
            {"order_id"},
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
