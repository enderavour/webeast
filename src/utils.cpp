#include "models/create.hpp"

void models::create_all_tables(orm::database &db)
{
    db.create_table<carts>();
    db.create_table<cart_items>();
    db.create_table<categories>();
    db.create_table<order_items>();
    db.create_table<orders>();
    db.create_table<products>();
    db.create_table<users>();
}
