#include "Interface.h"

#include "JsonReader.h"
#include "good_function.hpp"
#include "PostgresDB.h"
#include "PyLoader.h"

#include <iostream>
#include <locale>
#include <codecvt>

Interface::Interface(std::string offset)
{
    bot = std::make_unique<BotTelegram>(offset);
}

bool Interface::control_date()
{
    auto res = JsonReader::read(std::string("jq -r '.date' ../res/cards.json"), type_json::products);
    std::string date_str = res[0];
    std::istringstream ss(date_str);
    char delimiter;
    unsigned int day, month, year;
    ss >> day >> delimiter;
    if(delimiter != '-' || ss.fail())
        return false;
    ss >> month >> delimiter;
    if(delimiter != '-' || ss.fail())
        return false;
    ss >> year;
    if(ss.fail())
        return false;

    auto ymd = std::chrono::year{(int)year}/std::chrono::month{month}/std::chrono::day{day};
    auto current_date = get_date_now();

    std::chrono::sys_days new_date_days = std::chrono::sys_days{ymd} + std::chrono::days{7};
    std::chrono::sys_days my_date = std::chrono::sys_days{current_date};
    
    return my_date >= new_date_days;
}

bool Interface::start()
{
    bool flag = control_date();
    if(flag){
        int code_result_script = PyLoader::script_load(std::string("bash -c 'python3 ../py_scripts/script_requests.py'"));
        if(code_result_script == 1)
            PyLoader::script_load(std::string("bash -c 'python3 ../py_scripts/script_selenium.py'"));
        
        PostgresDB db;
        std::string conn = get_conn();
        db.connect(conn);

        std::ifstream file("../res/cards.json");
        nlohmann::json j = nlohmann::json::parse(file);

        std::string date = j["date"];

        for(const auto& card : j["cards"]){
            std::string str_name = card["name"];
            std::string str_price = card["price"];
            std::string str_sale = card["sale"];

            std::string name, price, sale;

            for(char c : str_price)
                if(isdigit(c))
                    price.push_back(c);

            for(char c : str_sale)
                if(isdigit(c))
                    sale.push_back(c);

            for(char c : str_name){
                if(c != 34)
                    name.push_back(c);
            }
            bot->notify_all(name);

            db.execute(std::string("INSERT INTO cards (name, price, discount, date) VALUES($1, $2, $3, $4);"), std::vector<std::string>{name, price, sale, date});
        }
        db.close();
    } else {
        std::ifstream file("../res/cards.json");
        nlohmann::json json = nlohmann::json::parse(file);
        std::string date = json["date"];

        PostgresDB db;
        std::string conn = get_conn();
        db.connect(conn);

        for(const auto& obj : json["cards"]){
            std::string str_name = obj["name"];
            std::string str_price = obj["price"];
            std::string str_sale = obj["sale"];

            std::string name, price, sale;

            for(char c : str_price)
                if(isdigit(c))
                    price.push_back(c);

            for(char c : str_sale)
                if(isdigit(c))
                    sale.push_back(c);

            for(char c : str_name){
                if(c != 34)
                    name.push_back(c);
            }

            // db.execute(std::string("INSERT INTO cards (name, price, discount, date) VALUES($1, $2, $3, $4);"), std::vector<std::string>{name, price, sale, date});

            bot->notify_all("3. " + name);

        }
        db.close();
    }
    return flag;
}