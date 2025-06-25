#include <iostream>
#include <fstream>
#include <ios>

#include "PostgresDB.h"
#include "good_function.hpp"

int main(void)
{
    std::ofstream file("../res/test_PostgresDB.txt", std::ios::app);
    if(!file.is_open()){
        std::cout << "Error open file\n";
        return 1;
    }
    file << "\n----- other test -----\n";
    std::string conn = get_conn();
    PostgresDB db;
    db.connect(conn);

    if(db.is_connect()){
        file << "All good with connect\n";
        auto res_1 = db.fetch(std::string("SELECT DISTINCT ON (date) date FROM cards ORDER BY date;"), std::vector<std::string>{});
        file << "Result of query - SELECT DISTINCT ON (date) date FROM cards ORDER BY date;:\n";
        for(int i = 0; i < res_1.size(); i++){
            file << '\t' << res_1[i][0] << '\n';
        }
        file << '\n';

        auto res_2 = db.fetch(std::string("SELECT price FROM cards;"), std::vector<std::string>{});
        file << "Result of - SELECT price FROM cards;:\n\t";
        for(int i = 0; i < res_2.size(); i++){
            file << res_2[i][0] << ' ';
        }
        file << '\n';

        std::string ex_3 = "discount";
        auto res_3 = db.fetch(std::string("SELECT $1 FROM cards;"), std::vector<std::string>{ex_3});
        file << "Result of - SELECT discount FROM cards;:\n\t";
        for(int i = 0; i < res_3.size(); i++){
            file << res_3[i][0] << ' ';
        }
        file << '\n';

        std::string ex_4 = "123456789";
        auto res_4 = db.execute(std::string("INSERT INTO users (id) VALUES ($1);"), std::vector<std::string>{ex_4});
        file << "Result of - INSERT INTO users (id) VALUES ('123456789');:\n\t" << res_4 << '\n';

        auto res_5 = db.execute(std::string("DELETE FROM users WHERE id = '123456789';"), std::vector<std::string>{});
        file << "Result of - DELETE FROM users WHERE id = '123456789';:\n\t" << res_5 << '\n';

    } else {
        file << "Error with connect\n";
    }
    db.close();
    if(db.is_connect()){
        file << "Error close connect\n";
    } else{
        file << "Good close connect\n";
    }
}