#ifndef CATEGORIES_SERVICE_IMPL
#define CATEGORIES_SERVICE_IMPL

#include "../../repositories/include/categories.hpp"
#include <optional>

class category_service
{
public:
    explicit category_service(orm::database &db);

    std::vector<categories> get_all();

    std::optional<categories> get_by_id(int32_t id);

    void update(int32_t id, const categories &category);

    void create(const categories &category);

    bool remove(int32_t id);
private:
    category_repository m_repository;
};

#endif
