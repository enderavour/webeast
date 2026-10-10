#ifndef USER_MODEL_IMPL_H
#define USER_MODEL_IMPL_H

#include <cstdint>
#include <string>
#include "../include/orm/orm.hpp"

enum class users_roles
{
    User,
    Admins
};

struct users
{
    int32_t id;
    std::string email;
    std::string password_hash;
    std::string first_name;
    std::string last_name;
    int32_t role;
};

template<>
struct orm::orm_model<users> : orm::orm_model_base<users>
{
    static constexpr const char *table = "users";

    static inline auto columns = std::make_tuple(
        orm::column("id", &users::id, orm::primary_key),
        orm::column("email", &users::email),
        orm::column("password_hash", &users::password_hash),
        orm::column("first_name", &users::first_name),
        orm::column("last_name", &users::last_name),
        orm::column("role", &users::role),
        orm::timestamp("created_at"),
        orm::timestamp("updated_at")
    );
};

#endif
