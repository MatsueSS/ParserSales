#include <iostream>
#include <fstream>

#include "PostgresDB.h"

int main(void)
{
    std::ifstream file("../.env");
    std::string temp, conn;
    getline(file, temp);
    getline(file, temp);
    getline(file, conn);

    PostgresDB db;
    db.connect(conn);

    std::ofstream test_file("../res/test_PostgresDB.txt");
    test_file << "Result is_connect with connect: " << db.is_connect() << '\n';

    db.close();
    test_file << "Result is_connect without connect: " << db.is_connect() << '\n';
    
    db.connect(conn);
    test_file << "Make a execute: " << db.execute(std::string("CREATE TABLE example (id SERIAL PRIMARY KEY);"), std::vector<std::string>{});
    db.execute(std::string("DROP TABLE example;"), std::vector<std::string>{});
}