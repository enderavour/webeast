#ifndef BIND_IMPL_H
#define BIND_IMPL_H

#include <sqlite3.h>
#include <type_traits>
#include <cstdint>
#include <string>

namespace orm
{

template<typename T>
void bind(sqlite3_stmt *stmt, int32_t index, const T &value);

template<typename T>
constexpr const char *get_sqlite_type();

template<typename T>
void bind_object(sqlite3_stmt *stmt, const T &obj);

};

#endif