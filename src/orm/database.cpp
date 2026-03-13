#include "../include/orm/database.hpp"
#include "../include/logger.hpp"
#include "../include/defs.hpp"

orm::Database::Database() = delete;
orm::Database::Database(const std::filesystem::path &path)
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

sqlite3 *orm::Database::handle() const
{
    return m_Conn;
}

orm::Database::~Database()
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

