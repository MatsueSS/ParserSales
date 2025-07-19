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

class TUexception : public std::exception{
protected:
    std::string msg;

public:
    TUexception(std::string);
    TUexception(const TUexception&);

    const char * what() const noexcept override;

};

class BadTypeValueTUexception : public TUexception{
public:
    BadTypeValueTUexception(std::string);

};

class EmptyQueryResultTUexception : public TUexception{
public:
    EmptyQueryResultTUexception(std::string);

};

class TelegramUser{
public:
    TelegramUser(const std::string& id) noexcept;

    TelegramUser(const TelegramUser&) = delete;
    TelegramUser& operator=(const TelegramUser&) = delete;

    TelegramUser(TelegramUser&&) noexcept;
    TelegramUser& operator=(TelegramUser&&) noexcept;

    bool operator==(const TelegramUser&) const noexcept;

    template<typename Type>
    void notify(Type&&);

    template<typename Type>
    void add_product(Type&&);

    template<typename Type>
    void del_product(Type&&);

    template<typename Type>
    double forecasting(Type&&);

    template<typename Type>
    bool is_has(Type&&) const;

    const std::unordered_set<std::string>& get_cards() const noexcept;

    int count_cards() const noexcept;

    std::string get_id() const noexcept;

private:
    std::string id;
    std::unordered_set<std::string> lovely_product;
};

template<typename Type>
void TelegramUser::add_product(Type&& product)
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueTUexception("Value must be string\n");
    }

    lovely_product.emplace(std::forward<Type>(product));
}

template<typename Type>
void TelegramUser::del_product(Type&& product)
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueTUexception("Value must be string\n");
    }

    lovely_product.erase(std::forward<std::string>(product));
}

template<typename Type>
void TelegramUser::notify(Type&& sale)
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueTUexception("Value must be string\n");
    }

    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::forward<Type>(sale));
}

template<typename Type>
double TelegramUser::forecasting(Type&& str)
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueTUexception("Value must be string\n");
    }

    PostgresDB db;
    std::string conn = get_conn();
    try{
        db.connect(conn);
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
    }

    std::vector<std::vector<std::string>> res;
    try{
        res = db.fetch(std::string("SELECT date FROM cards WHERE name = $1 ORDER BY date ASC;"), std::vector<std::string>{std::forward<Type>(str)});
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
        res = db.fetch(std::string("SELECT date FROM cards WHERE name = $1 ORDER BY date ASC;"), std::vector<std::string>{std::forward<Type>(str)});
    } catch(ErrorQueryResultDBexception& e) {
        std::cout << e.what() << '\n';
        res = db.fetch(std::string("SELECT date FROM cards WHERE name = $1 ORDER BY date ASC;"), std::vector<std::string>{std::forward<Type>(str)});
    }
    
    std::vector<std::chrono::year_month_day> dates;
    for(int i = 0; i < res.size(); i++){
        dates.push_back(to_date(res[i][0]));
    }

    std::vector<int> sample;
    for(int i = 1; i < res.size(); i++){
        sample.push_back(count_week(dates[i], dates[i-1]));
    }

    Forecast f;
    double probability;
    try{
        probability = f.make_forecast(sample);
    } catch(ZeroDivisionFCexception& e){
        probability = 0;
    }
    if(dates.empty())
        throw EmptyQueryResultTUexception("Bad query result\n");

    int n = count_week(get_date_now(), dates.back());
    return pow((1.0-probability), n)*probability;
}

template<typename Type>
bool TelegramUser::is_has(Type&& str) const
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueTUexception("Value must be string\n");
    }

    return lovely_product.count(str);
}

#endif //_TELEGRAM_USER_H_