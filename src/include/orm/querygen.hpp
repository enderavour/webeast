#ifndef QUERYGEN_IMPL_H
#define QUERYGEN_IMPL_H

#include <string>
#include <sqlite3.h>
#include <vector>
#include <string_view>
#include "bind.hpp"
#include "traits.hpp"

namespace orm
{

template<typename T>
std::string make_create_table_sql()
{
    std::string sql = "CREATE TABLE IF NOT EXISTS ";
    sql += orm::orm_traits<T>::table;
    sql += "(";

    bool first = true;

    std::apply([&](auto... col)
    {
        (
            [&]
            {
                using field_type = std::remove_cvref_t<
                    decltype(std::declval<T>().*(col.member))
                >;

                if (!first)
                    sql += ",";

                sql += col.name;
                sql += " ";
                sql += get_sqlite_type<field_type>();

                first = false;
            }(), ...
        );

    }, orm_traits<T>::columns);

    sql += ")";

    return sql;
}

template<typename T>
std::string make_insert_sql()
{
    std::string sql = "INSERT INTO ";
    sql += orm::orm_traits<T>::table;
    sql += " (";

    bool first = true;

    std::apply([&](auto... col)
    {
        ((sql += (first ? "" : ",") + std::string(col.name), first = false), ...);
    }, orm::orm_traits<T>::columns);

    sql += ") VALUES (";

    constexpr size_t N = std::tuple_size_v<decltype(orm::orm_traits<T>::columns)>;

    for (size_t i = 0; i < N; ++i)
    {
        if (i) sql += ",";
        sql += "?";
    }

    sql += ")";

    return sql;
}

// Queries

template<typename T>
void insert(sqlite3 *db, const T &obj)
{
    auto sql = make_insert_sql<T>();

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr
    );

    bind_object(stmt, obj);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

template<typename T>
T read_column(sqlite3_stmt *stmt, int32_t idx)
{
    if constexpr (std::is_same_v<T, int32_t>)
    {
        return sqlite3_column_int(stmt, idx);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        return sqlite3_column_double(stmt, idx);
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        const unsigned char *txt = sqlite3_column_text(stmt, idx);
        return txt ? reinterpret_cast<const char*>(txt) : "";
    }
}


template<typename T>
T read_row(sqlite3_stmt *stmt)
{
    T obj{};
    int32_t index = 0;

    std::apply([&](auto... col)
    {
        (void)std::initializer_list<int>
        {
            (obj.*(col.member) = read_column<
                std::remove_cvref_t<decltype(std::declval<T>().*(col.member))>
            >(stmt, index++), 0)...
        };
    }, orm::orm_traits<T>::columns);

    return obj;
}

template<typename T>
std::string make_select_sql()
{
    std::string sql = "SELECT ";
    bool first = true;

    std::apply([&](auto... col)
    {
        ((sql += (first ? "" : ",") + std::string(col.name), first = false), ...);
    }, orm::orm_traits<T>::columns);

    sql += " FROM ";
    sql += orm::orm_traits<T>::table;

    return sql;
}

template<typename T>
std::vector<T> select_all(sqlite3 *db)
{
    auto sql = make_select_sql<T>();

    sqlite3_stmt *stmt{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(get_config_opts(), std::format("Could not prepare SQL statement: {}", sqlite3_errmsg(db)));
        return {};
    }

    std::vector<T> result;

    while (true)
    {
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW)
        {
            result.emplace_back(read_row<T>(stmt));
        }
        else if (rc == SQLITE_DONE)
        {
            break;
        }
        else
        {
            LOG_ERROR(get_config_opts(), std::format("SQLite step error: {}", sqlite3_errmsg(db)));
            sqlite3_finalize(stmt);
            return {};
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

template<typename T>
std::vector<T> select_query(sqlite3 *db, std::string_view sql)
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(get_config_opts(), std::format("SQLite select error: {}", sqlite3_errmsg(db)));
        return {};
    }

    std::vector<T> result;

    while (true)
    {
        int32_t rc = sqlite3_step(stmt);

        switch (rc)
        {
            case SQLITE_ROW:
                result.emplace_back(read_row<T>(stmt));
                break;
            case SQLITE_DONE:
                break;
            default:
            LOG_ERROR(get_config_opts(), std::format("SQLite step error: {}", sqlite3_errmsg(db)));
            return {};
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

template<typename T>
void create_table(sqlite3 *db)
{
    auto sql = make_create_table_sql<T>();

    sqlite3_stmt *stmt;
    char *err;

    if (sqlite3_exec(db, sql.data(), nullptr, nullptr, &err) != SQLITE_OK)
    {
        std::string msg = err ? err : "Unknown SQL Error";
        LOG_ERROR(get_config_opts(), std::format("SQLite step error: {}", msg));
        sqlite3_free(err);
        return;
    }
}

template<typename T, typename ...Args>
std::vector<T> select_where(sqlite3 *db, std::string_view clause, Args&& ...args)
{
    std::string sql = make_select_sql<T>();
    sql += " ";
    sql += clause;

    sqlite3_stmt *stmt{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(get_config_opts(), std::format("Error preparing query: {}", sqlite3_errmsg(db)));
        return {};
    }

    int32_t index = 1;
    (void)std::initializer_list<int32_t>{(bind(stmt, index++, std::forward<Args>(args)), 0)...};

    std::vector<T> result;
    while (true)
    {
        int32_t rc = sqlite3_step(stmt);

        switch (rc)
        {
            case SQLITE_ROW:
                result.emplace_back(read_row<T>(stmt));
                break;
            case SQLITE_DONE:
                sqlite3_finalize(stmt);
                return result;
                break;
            default:
                LOG_ERROR(get_config_opts(), std::format("Step error: {}", sqlite3_errmsg(db)));
                sqlite3_finalize(stmt);
                return {};
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

}

#endif
