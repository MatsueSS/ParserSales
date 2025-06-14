#ifndef _TELEGRAM_USER_H_
#define _TELEGRAM_USER_H_

#include "json.hpp"
#include "TelegramSender.h"
#include "Forecast.hpp"

#include <unordered_set>
#include <string>

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
    void forecasting(Type&&);

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
    int val = sale[0] - '0';
    sale = sale.substr(3);
    switch(val)
    {
        case 1: //add_card
            lovely_product.emplace(std::forward<Type>(sale));
            break;
        case 2: //get_sales
        {
            std::ifstream file("../res/cards.json");
            nlohmann::json json = nlohmann::json::parse(file);
            auto ptr = TelegramSender::get_instance();
            for(const auto& obj : json["cards"]){
                std::string name = obj["name"];
                if(lovely_product.find(name) != lovely_product.end()){
                    ptr->call(std::string(id), type_msg::send, std::string(name));
                }
            }
            break;
        }
        case 3:
        {
            auto ptr = TelegramSender::get_instance();
            if(lovely_product.find(sale) != lovely_product.end())
                ptr->call(std::string(id), type_msg::send, std::string(sale));
        }
        case 4:
        {
            forecasting(sale);
        }
    }
}

template<typename Type>
void TelegramUser::forecasting(Type&& str)
{

}

#endif //_TELEGRAM_USER_H_