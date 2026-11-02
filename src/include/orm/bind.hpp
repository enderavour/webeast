#ifndef BIND_IMPL_H
#define BIND_IMPL_H

#include <sqlite3.h>
#include <type_traits>
#include <cstdint>
#include <string>
#include <chrono>
#include "model.hpp"
#include "../config.hpp"
#include "../defaults.hpp"
#include "../logger.hpp"
#include "timestamp.hpp"
#include "condition.hpp"

inline conf::config_opts &get_config_opts()
{
    static conf::config_opts opts = defaults::CONFIG.get_config_opts();
    return opts;
}

namespace orm
{

template<typename T>
int32_t bind(sqlite3_stmt *stmt, int32_t index, const T &value)
{
    using U = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<U, int32_t>)
    {
        return sqlite3_bind_int(stmt, index, value);
    }
    else if constexpr (std::is_same_v<U, int64_t>)
    {
        return sqlite3_bind_int64(stmt, index, value);
    }
    else if constexpr (std::is_same_v<U, double>)
    {
        return sqlite3_bind_double(stmt, index, value);
    }
    else if constexpr (std::is_same_v<U, float>)
    {
        return sqlite3_bind_double(stmt, index, value);
    }
    else if constexpr (std::is_same_v<U, bool>)
    {
        return sqlite3_bind_int(stmt, index, value ? 1 : 0);
    }
    else if constexpr (std::is_same_v<U, std::string>)
    {
        return sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    }
    else
    {
        LOG_ERROR("Unsupported bind type");
        static_assert(sizeof(T) == 0, "Unsupported bind type");
    }
}

template<typename Fields>
int32_t bind_fields(sqlite3_stmt *stmt, const Fields &fields)
{
    int32_t index =  1;
    int32_t rc = SQLITE_OK;

    std::apply([&](const auto& ...elems) {
        (
            [&] {
                if (rc != SQLITE_OK)
                    return;

                rc = orm::bind(stmt, index, elems.value);

                if (rc == SQLITE_OK)
                    index++;
            }(),
            ...
        );
    }, fields);

    return rc;
}

template<typename T, typename Member, typename Value>
int32_t bind_condition(
    sqlite3_stmt *stmt,
    const condition<T, Member, Value> &cond,
    int32_t index
) {
    return orm::bind(stmt, index, cond.value);
}

template<typename T>
constexpr const char *get_sqlite_type()
{
    using U = std::remove_cvref_t<T>;

    if constexpr (
        std::is_same_v<U, int32_t> ||
        std::is_same_v<U, int64_t> ||
        std::is_same_v<U, bool> ||
        std::is_same_v<U, std::chrono::time_point<std::chrono::system_clock>>
    )
        return "INTEGER";
    else if constexpr (
        std::is_same_v<U, double> || std::is_same_v<U, float>
    )
        return "REAL";
    else if constexpr (std::is_same_v<U, std::string>)
        return "TEXT";
    else
    {
        LOG_ERROR("Unsupported SQL type");
        static_assert(!std::is_same_v<U, U>, "Unsupported SQL type");
    }
}

template<typename T>
void bind_object(sqlite3_stmt *stmt, const T &obj)
{
    int32_t index = 1;

    std::apply([&](auto... col)
    {
        (
            [&]
            {
                if constexpr (!orm::is_timestamp_v<decltype(col)>)
                {
                    if (!(col.primary_key && col.autoincrement))
                    {
                        bind(
                            stmt,
                            index++,
                            obj.*(col.member)
                        );
                    }
                }
            }(), ...
        );
    }, orm::orm_model<T>::columns);
}

};

#endif
