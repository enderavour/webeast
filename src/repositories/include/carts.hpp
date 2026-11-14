#ifndef CARTS_REPOSITORY_IMPL
#define CARTS_REPOSITORY_IMPL

#include "../../include/orm/orm.hpp"
#include "../../models/carts.hpp"
#include <optional>


class carts_repository
{
public:
    explicit carts_repository(orm::database &db);

    void create(const carts &cart);
    std::optional<carts> get_by_id(int32_t carts_id);
    std::optional<carts> get_by_user_id(int32_t user_id);
    bool remove(int32_t id);
    bool remove_by_user_id(int32_t user_id);
private:
    orm::database &m_db;
};


#endif
