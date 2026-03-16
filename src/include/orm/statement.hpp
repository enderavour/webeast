#ifndef STATEMENT_IMPL_H
#define STATEMENT_IMPL_H

#include <sqlite3.h>
#include <string>

namespace orm
{
 
class Statement
{
public:
    Statement() = delete;
    Statement(sqlite3 *db, const std::string &sql_query)
    {
        sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);
    }

    bool step()
    {
        return sqlite3_step(stmt) == SQLITE_ROW;
    }
    sqlite3_stmt *raw() const
    {
        return stmt;
    }

    ~Statement()
    {
        if (stmt)
            sqlite3_finalize(stmt);
    }
private:
    sqlite3_stmt *stmt;
};

}

#endif