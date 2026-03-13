#ifndef STATEMENT_IMPL_H
#define STATEMENT_IMPL_H

#include <sqlite3.h>
#include <string>

namespace orm
{
 
class Statement
{
public:
    Statement();
    Statement(sqlite3 *db, const std::string &sql_query);
    ~Statement();
    bool step();
    sqlite3_stmt *raw() const;
private:
    sqlite3_stmt *stmt;
};

}

#endif