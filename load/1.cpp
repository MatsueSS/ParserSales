#include "PostgresDB.h"
#include "good_function.hpp"

int main(void)
{
    std::string conn = get_conn();
    PostgresDB db;
    db.connect(conn);
    db.execute(std::string("CREATE TABLE cards (name TEXT, price INT, discount INT, date DATE);"), std::vector<std::string>{});
    db.execute(std::string("CREATE TABLE users (id TEXT, cards TEXT[]);"), std::vector<std::string>{});
}