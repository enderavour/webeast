#include "../include/orm/traits.hpp"
#include "../include/orm/bind.hpp"
#include "../include/logger.hpp"
#include "../include/defs.hpp"
#include "../models/user.hpp"

template<typename T>
void orm::bind(sqlite3_stmt *stmt, int32_t index, const T &value)
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
#ifdef LOGGING_ENABLED_STDOUT
    logger::error("Unsupported bind type");
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, "Unsupported bind type");
#endif

        static_assert(sizeof(T) == 0, "Unsupported bind type");
    }
}

template<typename T>
constexpr const char *orm::get_sqlite_type()
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
#ifdef LOGGING_ENABLED_STDOUT
    logger::error("Unsupported SQL type");
#elifdef LOGGING_ENABLED_FILE
    logger::error(defaults::LOG_FILE_HANDLE, "Unsupported SQL type");
#endif
        static_assert(sizeof(T) == 0, "Unsupported SQL type");
    }
}

template<typename T>
void orm::bind_object(sqlite3_stmt *stmt, const T &obj)
{
    int32_t index = 1;

    std::apply([&](auto... col) 
    {
        ((bind(stmt, index++, obj.*(col.member))), ...);
    }, orm_traits<T>::columns);
}

template void orm::bind_object<User>(sqlite3_stmt *stmt, const User &obj);
template const char *orm::get_sqlite_type<std::string>();
template const char *orm::get_sqlite_type<int32_t>();
