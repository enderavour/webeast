#ifndef DATABASE_IMPL_H
#define DATABASE_IMPL_H

#include <sqlite3.h>
#include <filesystem>
#include "bind.hpp"
#include "querygen.hpp"

namespace orm
{

class database
{
public:
    database() = delete;
    database(const std::filesystem::path &path)
    {
        if (sqlite3_open(path.string().c_str(), &m_Conn) != SQLITE_OK)
        {
            LOG_ERROR(get_config_opts(), std::format("Could not open connection to database: {}", sqlite3_errmsg(m_Conn)));
        }
    }

    template<typename T>
    void insert(const T &obj)
    {
        orm::insert(m_Conn, obj);
    }

    template<typename T>
    void create_table()
    {
        orm::create_table<T>(m_Conn);
    }

    template<typename T>
    std::vector<T> select_all()
    {
        return orm::select_all<T>(m_Conn);
    }

    ~database()
    {
        LOG_INFO(get_config_opts(), "Closing database handle");
        sqlite3_close(m_Conn);
    }

    operator sqlite3*() const
    {
        return m_Conn;
    }
private:
    sqlite3 *m_Conn;
};

}
#endif
