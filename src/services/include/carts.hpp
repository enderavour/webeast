#ifndef CARTS_MODEL_SERVICE_IMPL
#define CARTS_MODEL_SERVICE_IMPL

#include "../../repositories/include/carts.hpp"

class carts_service
{
public:
    carts_service(orm::database &db);
    void create(const carts &cart);
    std::optional<carts> get_by_id(int32_t carts_id);
    std::optional<carts> get_by_user_id(int32_t user_id);
    bool remove(int32_t id);
    bool remove_by_user_id(int32_t user_id);
private:
    carts_repository _carts_repo;
};

#endif
