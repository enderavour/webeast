#ifndef MODEL_DEFS_INC_H
#define MODEL_DEFS_INC_H

#include "cart_items.hpp"
#include "carts.hpp"
#include "categories.hpp"
#include "order_items.hpp"
#include "orders.hpp"
#include "products.hpp"
#include "users.hpp"
#include "../include/orm/database.hpp"


namespace models
{
    void create_all_tables(orm::database &db);
};

#endif
