#ifndef CARTS_MODEL_IMPL_HPP
#define CARTS_MODEL_IMPL_HPP

#include <cstdint>
#include <tuple>
#include "../include/orm/orm.hpp"

struct carts
{
    int32_t id;
    int32_t user_id;
};

template<>
struct orm::orm_model<carts> : orm::orm_model_base<carts>
{
    static constexpr const char *table = "carts";

    static constexpr auto columns = std::make_tuple(
        orm::column("id", &carts::id, orm::primary_key),
        orm::column("user_id", &carts::user_id)
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
