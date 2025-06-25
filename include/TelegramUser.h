#ifndef _TELEGRAM_USER_H_
#define _TELEGRAM_USER_H_

#include "json.hpp"
#include "TelegramSender.h"
#include "Forecast.hpp"
#include "PostgresDB.h"
#include "good_function.hpp"

#include <unordered_set>
#include <string>
#include <iostream>
#include <cmath>

class TelegramUser{
public:
    TelegramUser(const std::string& id);

    TelegramUser(const TelegramUser&) = delete;
    TelegramUser& operator=(const TelegramUser&) = delete;

    TelegramUser(TelegramUser&&) noexcept;
    TelegramUser& operator=(TelegramUser&&) noexcept;

    template<typename Type>
    void notify(Type&&);

    template<typename Type>
    void add_product(Type&&);

    template<typename Type>
    void del_product(Type&&);

    template<typename Type>
    double forecasting(Type&&);

    const std::unordered_set<std::string>& get_cards() const;

    std::string get_id() const;

private:
    std::string id;
    std::unordered_set<std::string> lovely_product;
};

template<typename Type>
void TelegramUser::add_product(Type&& product)
{
    lovely_product.emplace(std::forward<Type>(product));
}

template<typename Type>
void TelegramUser::del_product(Type&& product)
{
    lovely_product.erase(std::forward<std::string>(product));
}

template<typename Type>
void TelegramUser::notify(Type&& sale)
{
    if(lovely_product.count(sale)){
        auto ptr = TelegramSender::get_instance();
        std::cout << sale << '\n';
        ptr->call(std::string(id), type_msg::send, std::string(sale));
    }
}

template<typename Type>
double TelegramUser::forecasting(Type&& str)
{
    PostgresDB db;
    std::string conn = get_conn();
    db.connect(conn);

    auto res = db.fetch(std::string("SELECT date FROM cards WHERE name = $1 ORDER BY date ASC;"), std::vector<std::string>{std::forward<Type>(str)});

    std::vector<std::chrono::year_month_day> dates;
    for(int i = 0; i < res.size(); i++){
        dates.push_back(to_date(res[i][0]));
    }

    std::vector<int> sample;
    for(int i = 1; i < res.size(); i++){
        sample.push_back(count_week(dates[i], dates[i-1]));
    }
    
    Forecast f;
    double probability = f.make_forecast(sample);
    int n = count_week(get_date_now(), dates.back());
    return pow((1.0-probability), n)*probability;
}

#endif //_TELEGRAM_USER_H_