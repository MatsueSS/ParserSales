#ifndef _GOOD_FUNCTION_HPP_
#define _GOOD_FUNCTION_HPP_

#include <chrono>
#include <string>

std::string get_conn();

std::chrono::year_month_day get_date_now();

std::chrono::year_month_day to_date(std::string str);

int count_week(std::chrono::year_month_day obj1, std::chrono::year_month_day obj2);

#endif //_GOOD_FUNCTION_HPP