#include <iostream>
#include <fstream>

#include "PostgresDB.h"

int main(void)
{
    std::ifstream file("../.env");
    std::string temp, conn, other_connection;
    getline(file, temp);
    getline(file, temp);
    getline(file, conn);
    getline(file, other_connection);

    PostgresDB db;
    db.connect(conn);

    std::ofstream test_file("../res/test_PostgresDB.txt");
    test_file << "Result is_connect with connect: " << db.is_connect() << '\n';

    db.close();
    test_file << "Result is_connect without connect: " << db.is_connect() << '\n';
    
    db.connect(conn);
    test_file << "Make a execute: " << db.execute(std::string("CREATE TABLE example (id SERIAL PRIMARY KEY);"), std::vector<std::string>{}) << '\n';
    db.execute(std::string("DROP TABLE example;"), std::vector<std::string>{});

    db.close();
    db.connect(other_connection);

    std::vector<std::vector<std::string>> result_of_query = db.fetch(
        std::string("SELECT * FROM data;"),
        std::vector<std::string>{}
    );

    test_file << "Result of query-'SELECT * FROM data;':" << '\n';

    for(const auto& row : result_of_query){
        test_file << '\t';
        for(const auto& obj : row){
            test_file << obj << ' ';
        }
        test_file << '\n';
    }

    return 0;
}