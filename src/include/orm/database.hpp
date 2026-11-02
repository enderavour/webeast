#ifndef DATABASE_IMPL_H
#define DATABASE_IMPL_H

#include <sqlite3.h>
#include <filesystem>
#include "bind.hpp"
#include "querygen.hpp"
#include <format>
#include <stdexcept>


namespace orm
{

class database
{
public:
    database() = delete;

    // Removing copy constructor and copy assignment operator in order to avoid double ownership and double free
    database(const database&) = delete;
    database operator=(const database&) = delete;

    database(const std::filesystem::path &path)
    {
        if (sqlite3_open(path.string().c_str(), &m_Conn) != SQLITE_OK)
        {
            LOG_ERROR(std::format("Could not open connection to database: {}", sqlite3_errmsg(m_Conn)));
        }

        // Turn on foreign key check for every connection
        if (sqlite3_exec(m_Conn, "PRAGMA foreign_keys = ON", nullptr, nullptr, nullptr) != SQLITE_OK)
        {
            std::string err = sqlite3_errmsg(m_Conn);

            sqlite3_close(m_Conn);
            m_Conn = nullptr;

            throw std::runtime_error(
                std::format(
                    "Could not enable foreign keys: {}",
                    err
                )
            );
        }
    }

    template<typename T>
    void insert(const T &obj)
    {
        orm::insert<T>(m_Conn, obj);
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

    template<typename T, typename ...Args>
    std::vector<T> select(std::string_view clause, Args&& ...args)
    {
        return orm::select_where<T, Args...>(m_Conn, clause, std::forward<Args>(args)...);
    }

    template<typename T, typename Field>
    void update(const Field &field)
    {
        orm::update<T>(m_Conn, field);
    }

    template<typename T, typename Field, typename Condition>
    void update(const Field &field, const Condition &condition)
    {
        orm::update<T>(m_Conn, field, condition);
    }

    template<typename T, typename Condition>
    bool remove(const Condition &condition)
    {
        return orm::remove<T>(m_Conn, condition);
    }

    ~database()
    {
        LOG_INFO("Closing database handle");
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
