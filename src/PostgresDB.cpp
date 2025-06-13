#include "PostgresDB.h"

void PostgresDB::close()
{
    if(conn)
        conn.reset();
}

bool PostgresDB::is_connect() const
{
    return PQstatus(conn.get()) == CONNECTION_OK;
}

PostgresDB::~PostgresDB()
{
    this->close();
}

PostgresDB::PostgresDB(PostgresDB&& obj) noexcept : conn(std::move(obj.conn)) {}

PostgresDB& PostgresDB::operator=(PostgresDB&& obj) noexcept
{
    if(this == &obj)
        return *this;

    conn = std::move(obj.conn);
    return *this;
}