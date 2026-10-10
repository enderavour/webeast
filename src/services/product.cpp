#include "include/product.hpp"

product_service::product_service(orm::database &db): m_repository(db) {}

std::vector<products> product_service::get_all()
{
    return m_repository.get_all();
}

std::optional<products> product_service::get_by_id(int32_t id)
{
    return m_repository.get_by_id(id);
}
