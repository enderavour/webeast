#include "include/categories.hpp"
#include <optional>

category_service::category_service(orm::database &db): m_repository(db) {}

std::vector<categories> category_service::get_all()
{
    return m_repository.select_all();
}

std::optional<categories> category_service::get_by_id(int32_t id)
{
    return m_repository.select_one(id);
}

void category_service::create(const categories &category)
{
    m_repository.create(category);
}

void category_service::update(int32_t id, const categories &category)
{
    m_repository.update(id, category);
}

bool category_service::remove(int32_t id)
{
    return m_repository.remove(id);
}
