#ifndef BIND_IMPL_H
#define BIND_IMPL_H

#include <sqlite3.h>
#include <type_traits>
#include <cstdint>
#include <string>
#include "traits.hpp"
#include "../config.hpp"
#include "../defs.hpp"
#include "../logger.hpp"

inline conf::config_opts &get_config_opts()
{
    static conf::config_opts opts = defaults::CONFIG.get_config_opts();
    return opts;
}

namespace orm
{

template<typename T>
void bind(sqlite3_stmt *stmt, int32_t index, const T &value)
{
    if constexpr (std::is_same_v<T, int32_t>)
    {
        sqlite3_bind_int(stmt, index, value);
    }
    else if constexpr (std::is_same_v<T, int64_t>)
    {
        sqlite3_bind_int64(stmt, index, value);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        sqlite3_bind_double(stmt, index, value);
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    }
    else
    {
        LOG_ERROR(get_config_opts(), "Unsupported bind type");
        static_assert(sizeof(T) == 0, "Unsupported bind type");
    }
}

template<typename T>
constexpr const char *get_sqlite_type()
{
    if constexpr (
        std::is_same_v<T, int32_t> ||
        std::is_same_v<T, int64_t> ||
        std::is_same_v<T, bool>
    )
        return "INTEGER";
    else if constexpr (
        std::is_same_v<T, double> || std::is_same_v<T, float>
    )
        return "REAL";
    else if constexpr (std::is_same_v<T, std::string>)
        return "TEXT";
    else
    {
        LOG_ERROR(get_config_opts(), "Unsupported SQL type");
        static_assert(sizeof(T) == 0, "Unsupported SQL type");
    }
}

template<typename T>
void bind_object(sqlite3_stmt *stmt, const T &obj)
{
    int32_t index = 1;

    std::apply([&](auto... col)
    {
        ((bind(stmt, index++, obj.*(col.member))), ...);
    }, orm_traits<T>::columns);
}

};

#endif
