#ifndef PRODUCTS_IMPL_HPP
#define PRODUCTS_IMPL_HPP

#include <cstdint>
#include <string>
#include "../include/orm/orm.hpp"

struct products
{
    int32_t id;
    int32_t category_id;
    std::string name;
    std::string description;
    int32_t price;
    int32_t stock;
};


template<>
struct orm::orm_model<products> : orm::orm_model_base<products>
{
    static constexpr const char *table = "products";

    static inline auto columns = std::make_tuple(
        orm::column("id", &products::id, orm::primary_key),
        orm::column("category_id", &products::category_id),
        orm::column("name", &products::name),
        orm::column("description", &products::description),
        orm::column("price", &products::price),
        orm::column("stock", &products::stock)
    );

    static inline auto constraints = std::make_tuple(
        orm::foreign_key(
            "categories",
            {"category_id"},
            {"id"}
        )
    );
};

#endif
