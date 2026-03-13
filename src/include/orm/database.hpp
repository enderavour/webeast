#ifndef DATABASE_IMPL_H
#define DATABASE_IMPL_H

#include <sqlite3.h>
#include <filesystem>

namespace orm
{

class Database
{
public:
    Database();
    Database(const std::filesystem::path &path);
    ~Database();
    sqlite3 *handle() const;
private:
    sqlite3 *m_Conn;
};

}
#endif