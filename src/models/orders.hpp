#ifndef ORDERS_MODEL_IMPL_HPP
#define ORDERS_MODEL_IMPL_HPP

#include <cstdint>
#include "../include/orm/orm.hpp"

struct orders
{
    int32_t id;
    int32_t user_id;
    int32_t total_price;
    int32_t status;
};

template<>
struct orm::orm_model<orders> : orm::orm_model_base<orders>
{
    static constexpr const char *table = "orders";

    static constexpr auto columns = std::make_tuple(
        orm::column("id", &orders::id, orm::primary_key),
        orm::column("user_id", &orders::user_id),
        orm::column("total_price", &orders::total_price),
        orm::column("status", &orders::status)
    );

    static inline auto constraints = std::make_tuple(
        orm::foreign_key(
            "users",
            {"user_id"},
            {"id"}
        )
    );
};

#endif
