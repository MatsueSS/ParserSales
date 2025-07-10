#ifndef _GOOD_FUNCTION_HPP_
#define _GOOD_FUNCTION_HPP_

#include "PostgresDB.h"

#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

const std::string path = "/home/michael/practisecpp/projects/ParserSales/";

std::string get_conn();

std::chrono::year_month_day get_date_now();

std::chrono::year_month_day to_date(std::string str);

int count_week(std::chrono::year_month_day obj1, std::chrono::year_month_day obj2);

std::string read_offset();

template<typename Type>
void offset_reload(Type&& str)
{
    std::ifstream old_file(path+".env");
    std::string temp;
    std::vector<std::string> arr;
    while(std::getline(old_file, temp))
        arr.push_back(temp);

    old_file.close();

    std::ofstream new_file(path+".env");
    for(int i = 0; i < arr.size() - 1; i++)
        new_file << arr[i] << '\n';

    new_file << str << '\n';

    new_file.close();
}

template<typename Func>
void retry_db_operation(PostgresDB& db, const std::string& conn, Func op)
{
    try{
        db.connect(conn);
        op();
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
        op();
    } catch(ErrorQueryResultDBexception& e) {
        std::cout << e.what() << '\n';
        op();
    }
}



#endif //_GOOD_FUNCTION_HPP