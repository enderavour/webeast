#ifndef CATEGORIES_IMPL_H
#define CATEGORIES_IMPL_H

#include <cstdint>
#include <string>
#include "../include/orm/orm.hpp"

struct categories
{
    int32_t id;
    std::string name;
    std::string description;
};

template<>
struct orm::orm_model<categories> : orm::orm_model_base<categories>
{
    static constexpr const char *table = "categories";

    static inline auto columns = std::make_tuple(
        orm::column("id", &categories::id, orm::primary_key),
        orm::column("name", &categories::name),
        orm::column("description", &categories::description)
    );
};

#endif
