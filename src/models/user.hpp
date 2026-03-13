#ifndef USER_MODEL_IMPL_H
#define USER_MODEL_IMPL_H

#include <cstdint>
#include <string>
#include "../include/orm/orm.hpp"

struct User
{
    int32_t id;
    std::string name;
};

template<>
struct orm::orm_traits<User>
{
    static constexpr const char *table = "User";

    static inline auto columns = std::make_tuple(
        orm::column("id", &User::id),
        orm::column("name", &User::name)
    );
};

#endif