#ifndef PRODUCT_SERVICE_IMPL_HPP
#define PRODUCT_SERVICE_IMPL_HPP

#include "../../repositories/include/product.hpp"
#include "../../include/orm/orm.hpp"
#include <optional>

class product_service
{
public:
    explicit product_service(orm::database &db);

    std::vector<products> get_all();

    std::optional<products> get_by_id(int32_t id);
private:
    product_repository m_repository;
    void create(const products &product);
};

#endif
