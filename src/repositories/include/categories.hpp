#ifndef CATEGORY_REPOSITORY_IMPL
#define CATEGORY_REPOSITORY_IMPL

#include "../../include/orm/orm.hpp"
#include "../../models/categories.hpp"
#include "../../models/create.hpp"
#include <vector>
#include <optional>

class category_repository
{
public:
    explicit category_repository(orm::database &db);
    std::vector<categories> select_all();
    std::optional<categories> select_one(int32_t id);

    void update(int32_t id, const categories &category);

    void create(const categories &category);

    bool remove(int32_t id);
private:
    orm::database &m_db;
};


#endif
