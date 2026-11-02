#ifndef QUERYGEN_IMPL_H
#define QUERYGEN_IMPL_H

#include <cstddef>
#include <string>
#include <format>
#include <sqlite3.h>
#include <type_traits>
#include <vector>
#include <string_view>
#include <algorithm>
#include "bind.hpp"
#include "column.hpp"
#include "column_name.hpp"
#include "condition.hpp"
#include "timestamp.hpp"
#include "model.hpp"
#include "foreignkey.hpp"
#include "../logger.hpp"
#include "../defaults.hpp"
#include <iostream>

namespace orm
{

template<typename T>
std::string make_create_table_sql()
{
    std::string sql = "CREATE TABLE IF NOT EXISTS ";
    sql += orm::orm_model<T>::table;
    sql += "(";

    bool first = true;

    std::apply([&](auto... col)
    {
        (
            [&]
            {


                if (!first)
                    sql += ",";

                sql += col.name;
                sql += " ";

                if constexpr (orm::is_timestamp_v<decltype(col)>)
                {
                    sql += "INTEGER NOT NULL DEFAULT(unixepoch())";
                }

                else
                {
                    using field_type = std::remove_cvref_t<
                        decltype(std::declval<T>().*(col.member))
                    >;

                    sql += get_sqlite_type<field_type>();

                    if (col.primary_key)
                        sql += " PRIMARY KEY";

                    if (col.primary_key && col.autoincrement)
                        sql += " AUTOINCREMENT";

                }

                first = false;
            }(), ...
        );

    }, orm_model<T>::columns);

    std::apply([&](auto... cn)
    {
        (
            [&]
            {
                if (orm::is_fk_v<decltype(cn)>)
                {
                    if (!first)
                        sql += ",";

                    std::string cols, ref_cols;

                    std::for_each(cn.columns.begin(), cn.columns.end(), [&cols](std::string_view c){
                        cols += c;
                        cols += ",";
                    });
                    cols.pop_back(); // remove trailing comma after the last element was pushed

                    std::for_each(cn.referenced_columns.begin(), cn.referenced_columns.end(), [&ref_cols](std::string_view c){
                        ref_cols += c;
                        ref_cols += ",";
                    });
                    ref_cols.pop_back(); // remove trailing comma after the last element was pushed

                    sql += std::format(
                        "FOREIGN KEY ({}) REFERENCES {}({})",
                        cols, cn.referenced_table, ref_cols
                    );
                }

                first = false;
            }(), ...
        );
    }, orm_model<T>::constraints);

    sql += ")";

    return sql;
}

template<typename T>
std::string make_insert_sql()
{
    std::string sql = "INSERT INTO ";
    sql += orm::orm_model<T>::table;
    sql += " (";

    bool first = true;
    size_t count = 0;

    std::apply([&](auto... col)
    {
        (
            [&]
            {
                if (!orm::is_timestamp_v<decltype(col)>)
                {
                    if (!(col.primary_key && col.autoincrement))
                    {
                        if (!first)
                            sql += ",";

                        sql += col.name;
                        first = false;
                        ++count;
                    }
                }

            }(), ...
        );
    }, orm::orm_model<T>::columns);

    sql += ") VALUES (";


    for (size_t i = 0; i < count; ++i)
    {
        if (i) sql += ",";
        sql += "?";
    }

    sql += ")";

    return sql;
}

// Queries
template<typename T>
void insert(sqlite3* db, const T& obj)
{
    const auto sql = make_insert_sql<T>();

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr
    );

    if (rc != SQLITE_OK)
    {
        throw std::runtime_error(
            std::string("SQLite prepare failed: ") +
            sqlite3_errmsg(db) +
            " | SQL: " + sql
        );
    }

    try
    {
        bind_object(stmt, obj);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            throw std::runtime_error(
                std::string("SQLite insert failed: ") +
                sqlite3_errmsg(db) +
                " | SQL: " + sql
            );
        }

        const auto rowid = sqlite3_last_insert_rowid(db);

        conf::config_opts copts =
            defaults::CONFIG.get_config_opts();

        LOG_INFO(
            std::format(
                "INSERT succeeded. rowid={}, SQL={}",
                rowid,
                sql
            )
        );
    }
    catch (...)
    {
        sqlite3_finalize(stmt);
        throw;
    }

    rc = sqlite3_finalize(stmt);

    if (rc != SQLITE_OK)
    {
        throw std::runtime_error(
            std::string("SQLite finalize failed: ") +
            sqlite3_errmsg(db)
        );
    }
}

// Conditional update
template<typename T, typename Field, typename Condition>
int32_t update(
  sqlite3 *db,
  const Field &field,
  const Condition &condition
) {
    std::string sql = "UPDATE ";
    sql += orm::orm_model<T>::table;

    bool first = true;

    sql += " SET ";

    try
    {
        std::apply([&sql, &first](const auto&... elems) {
            (
                [&](const auto &elem) {
                    if (!first) {
                        sql += ", ";
                    }

                    first = false;

                    auto column_name = orm::column_name(elem.member);
                    sql += std::format("{} = ?", column_name);
                }(elems),
                ...
            );
        }, field);

        sql += orm::where_clause(condition);
    } catch (const std::runtime_error &e)
    {
        LOG_ERROR(std::format("update: {}", e.what()));
        return SQLITE_ERROR;
    }

    sql += ";";

    sqlite3_stmt *stmt;

    int32_t rc = 0;

    if ((rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)) != SQLITE_OK)
    {
        LOG_ERROR(std::format("update: Error preparing the SQL statement: {}", sqlite3_errmsg(db)));
        return rc;
    }

    rc = orm::bind_fields(stmt, field);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format(
            "update: failed to bind fields: {}",
            sqlite3_errmsg(db)
        ));
        sqlite3_finalize(stmt);
        return rc;
    }

    const int32_t where_index = static_cast<int32_t>(std::tuple_size_v<Field>) + 1;

    rc = bind_condition(stmt, condition, where_index);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format("update: failed to bind condition: {}", sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return rc;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LOG_ERROR(std::format("update: Error executing the SQL query: {}", sqlite3_errmsg(db)));

        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}


// Unconditional update
template<typename T, typename Field>
int32_t update(
  sqlite3 *db,
  const Field &field
) {
    std::string sql = "UPDATE ";
    sql += orm::orm_model<T>::table;
    bool first = true;

    sql += " SET ";

    try
    {
        std::apply([&sql, &first](const auto&... elems) {
            (
                [&](const auto &elem) {
                    if (!first) {
                        sql += ", ";
                    }

                    first = false;

                    auto column_name = orm::column_name(elem.member);
                    sql += std::format("{} = ?", column_name);
                }(elems),
                ...
            );
        }, field);
    } catch (const std::runtime_error &e)
    {
        LOG_ERROR(std::format("update: {}", e.what()));
        return SQLITE_ERROR;
    }

    if (first)
    {
        LOG_ERROR("update: no fields to update");
        return SQLITE_DONE;
    }

    sql += ";";

    sqlite3_stmt *stmt = nullptr;
    int32_t rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format("update: Error preparing the SQL statement: {}", sqlite3_errmsg(db)));

        if (stmt)
            sqlite3_finalize(stmt);

        return rc;
    }

    rc = orm::bind_fields(stmt, field);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format(
            "update: failed to find binds: {}",
            sqlite3_errmsg(db)
        ));
        sqlite3_finalize(stmt);
        return rc;
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_ERROR(std::format("update: Error executing the SQL query: {}", sqlite3_errmsg(db)));

        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
}

template<typename T, typename Condition>
bool remove(
    sqlite3 *db,
    const Condition &condition
)
{
    std::string sql = " DELETE FROM ";

    sql += orm::orm_model<T>::table;
    sql += orm::where_clause(condition);
    sql += ";";

    sqlite3_stmt *stmt = nullptr;

    int32_t rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format("remove: failed to prepare statement: {}", sqlite3_errmsg(db)));

        if (stmt)
            sqlite3_finalize(stmt);

        return false;
    }

    orm::bind_condition(stmt, condition, 1);

    if (rc != SQLITE_OK)
    {
        LOG_ERROR(std::format("remove: failed to bind condition: {}", sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_ERROR(std::format("remove: failed to execute query: {}", sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
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
        (
            [&]
            {
                if constexpr (!orm::is_timestamp_v<decltype(col)>)
                {
                    obj.*(col.member) = read_column<
                        std::remove_cvref_t<
                            decltype(
                                std::declval<T>().*(col.member)
                            )
                        >
                    >(stmt, index++);
                }
            }(), ...
        );
    }, orm::orm_model<T>::columns);

    return obj;
}

template<typename T>
std::string make_select_sql()
{
    std::string sql = "SELECT ";
    bool first = true;

    std::apply([&](auto... col)
    {
        (
            [&]
            {
                if constexpr (!orm::is_timestamp_v<decltype(col)>)
                {
                    if (!first)
                        sql += ",";

                    sql += col.name;
                    first = false;
                }
            }(), ...
        );
    }, orm::orm_model<T>::columns);

    sql += " FROM ";
    sql += orm::orm_model<T>::table;

    return sql;
}

template<typename T>
std::vector<T> select_all(sqlite3 *db)
{
    auto sql = make_select_sql<T>();

    sqlite3_stmt *stmt{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(std::format("Could not prepare SQL statement: {}", sqlite3_errmsg(db)));
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
            LOG_ERROR(std::format("SQLite step error: {}", sqlite3_errmsg(db)));
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
        LOG_ERROR(std::format("SQLite select error: {}", sqlite3_errmsg(db)));
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
                sqlite3_finalize(stmt);
                return result;
            default:
                LOG_ERROR(std::format("SQLite step error: {}", sqlite3_errmsg(db)));
                sqlite3_finalize(stmt);
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
        LOG_ERROR(std::format("SQLite step error: {}", msg));
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
        LOG_ERROR(std::format("Error preparing query: {}", sqlite3_errmsg(db)));
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
                LOG_ERROR(std::format("Step error: {}", sqlite3_errmsg(db)));
                sqlite3_finalize(stmt);
                return {};
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

}

#endif
