#ifndef PRODUCT_REPOSITORY_IMPL_H
#define PRODUCT_REPOSITORY_IMPL_H

#include "../../include/orm/orm.hpp"
#include "../../models/create.hpp"

class product_repository
{
public:
    explicit product_repository(orm::database &db);

    std::vector<products> get_all();

    std::optional<products> get_by_id(int32_t id);

    void create(const products &product);
private:
    orm::database &m_db;
};


#endif
