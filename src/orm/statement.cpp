#include "../include/orm/statement.hpp"

orm::Statement::Statement() = delete;

orm::Statement::Statement(sqlite3 *db, const std::string &sql_query)
{
    sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);
}

bool orm::Statement::step()
{
    return sqlite3_step(stmt) == SQLITE_ROW;
}

sqlite3_stmt *orm::Statement::raw() const
{
    return stmt;
}
