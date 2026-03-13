#ifndef QUERYGEN_IMPL_H
#define QUERYGEN_IMPL_H

#include <string>
#include <sqlite3.h>
#include <vector>
#include <string_view>

namespace orm
{

template<typename T>
std::string make_create_table_sql();

template<typename T>
std::string make_insert_sql();

// Queries

template<typename T>
void insert(sqlite3 *db, const T &obj);

template<typename T>
T read_column(sqlite3_stmt *stmt, int32_t idx);

template<typename T>
T read_row(sqlite3_stmt *stmt);

template<typename T>
std::string make_select_sql();

template<typename T>
std::vector<T> select_all(sqlite3 *db);

template<typename T>
std::vector<T> select_query(sqlite3 *db, std::string_view sql);

template<typename T>
void create_table(sqlite3 *db);

}

#endif