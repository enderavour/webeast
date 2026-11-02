#include "include/categories.hpp"

category_repository::category_repository(orm::database &db): m_db(db) {}

std::vector<categories> category_repository::select_all()
{
    return m_db.select_all<categories>();
}

std::optional<categories> category_repository::select_one(int32_t id)
{
    auto selected_categories = m_db.select<categories>("WHERE id = ?", id);
    if (selected_categories.empty())
        return std::nullopt;

    return selected_categories[0];
}

void category_repository::create(const categories &category)
{
    m_db.insert<categories>(category);
}

void category_repository::update(int32_t id, const categories &category)
{
    auto fields = orm::set(
        orm::set_field(&categories::name, category.name),
        orm::set_field(&categories::description, category.description)
    );

    auto update_field = orm::where(&categories::id, id);

    m_db.update<categories>(fields, update_field);
}

bool category_repository::remove(int32_t id)
{
    auto condition = orm::where(&categories::id, id);

    return m_db.remove<categories>(condition);
}
