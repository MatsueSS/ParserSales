#include "good_function.hpp"

#include <fstream>
#include <sstream>

std::string get_conn(){
    std::ifstream file("../.env");
    std::string temp, conn;
    getline(file, temp);
    getline(file, temp);
    getline(file, conn);
    return conn;
}

std::chrono::year_month_day get_date_now()
{
    auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    auto current_date = std::chrono::year_month_day{today};
    return current_date;
}

std::chrono::year_month_day to_date(std::string str)
{
    std::istringstream iss(str);
    int year_int;
    unsigned int month_int, day_int;
    char sep1, sep2;
    iss >> year_int >> sep1 >> month_int >> sep2 >> day_int;
    return std::chrono::year_month_day{
        std::chrono::year{year_int}, 
        std::chrono::month{month_int},
        std::chrono::day{day_int}};
}

int count_week(std::chrono::year_month_day obj1, std::chrono::year_month_day obj2)
{
    std::chrono::sys_days sd1 = std::chrono::sys_days{obj1};
    std::chrono::sys_days sd2 = std::chrono::sys_days{obj2};

    auto diff_days = (sd2 - sd1).count();
    return diff_days / 7;
}