#ifndef DATABASE_IMPL_H
#define DATABASE_IMPL_H

#include <sqlite3.h>
#include <filesystem>
#include "traits.hpp"

namespace orm
{

class Database
{
public:
    Database() = delete;
    Database(const std::filesystem::path &path)
    {
        if (sqlite3_open(path.c_str(), &m_Conn) != SQLITE_OK)
        {
    #ifdef LOGGING_ENABLED_STDOUT
        logger::error(
            std::format("Could not open connection to database: {}", sqlite3_errmsg(m_Conn))
        );
    #elifdef LOGGING_ENABLED_FILE
        logger::error(
            defaults::LOG_FILE_HANDLE,
            std::format("Could not open connection to database: {}", sqlite3_errmsg(m_Conn))
        );
    #endif
        }
    }
    
    ~Database()
    {
    #ifdef LOGGING_ENABLED_STDOUT
        logger::info("Closing database handle");
    #elifdef LOGGING_ENABLED_FILE
        logger::error(
            defaults::LOG_FILE_HANDLE,
            "Closing database handle"
        );
    #endif
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