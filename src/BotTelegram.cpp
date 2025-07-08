#include "BotTelegram.h"
#include "JsonReader.h"
#include "TelegramSender.h"
#include "PostgresDB.h"

#include "good_function.hpp"
#include "JsonReader.h"
#include "json.hpp"
#include "TelegramSender.h"

#include <thread>
#include <iostream>
#include <regex>

BotTelegram::BotTelegram(std::string offset) : offset(offset)
{
    std::thread thr(&BotTelegram::check_msg, this);
    thr.detach();

    std::string conn = get_conn();
    PostgresDB db;
    db.connect(conn);
    auto res = db.fetch(std::string("SELECT id, cards FROM users;"), std::vector<std::string>{});
    for(const auto& row : res){
        std::string link = row[1], id = row[0];
        std::unique_ptr<TelegramUser> user = std::make_unique<TelegramUser>(id);
        std::regex elementRegex(R"(\"([^"]+)\")");
        std::smatch match;
        auto begin = link.cbegin();
        auto end = link.cend();
        while(std::regex_search(begin, end, match, elementRegex)){
            user->add_product(match[1]);
            begin = match.suffix().first;
        }
        users.emplace(std::move(user));
    }
}

void BotTelegram::check_msg()
{
    std::string id, command, data;
    while(!stop_flag.load()){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(std::string(""), type_msg::read, std::string(offset));
        auto v = JsonReader::read("jq -r '.result[] | {text: .message.text, id: .message.from.id}' ../res/result_"+ offset +".json", type_json::message);
        if(!v.empty())
        {
            id = v[2];
            
            std::size_t spacePos = v[1].find(' ');
            if(spacePos != std::string::npos){
                command = v[1].substr(0, spacePos);
                data = v[1].substr(spacePos+1);
            } else {
                command = v[1];
            }

            if(command == "/start"){
                users.emplace(std::make_unique<TelegramUser>(id));
            }
            else if(command == "/status"){
                auto user = find_user(std::move(id));
                std::ifstream file("../res/cards.json");
                nlohmann::json j = nlohmann::json::parse(file);

                std::string date = j["date"];

                for(const auto& card : j["cards"]){
                    std::string str_name = card["name"];
                    if(user->is_has(str_name))
                        user->notify(str_name);
                }
            }
            else if(command == "/forecast"){
                auto user = find_user(id);
                double probability = user->forecasting(data);
                user->notify(std::string("Вероятность скидки на следующей неделе равна: ") + std::to_string(probability));
            }
            else if(command == "/add_card"){
                auto user = find_user(id);
                PostgresDB db;
                std::string conn = get_conn();
                db.connect(conn);
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                user->add_product(data);
            }
            else if(command == "/my_cards"){
                auto ptr = TelegramSender::get_instance();
                auto user = find_user(id);
                auto cards = user->get_cards();
                for(auto iter = cards.begin(); iter != cards.end(); iter++)
                    user->notify(*iter);
            }
            else if(command == "/del_cards"){
                auto user = find_user(id);
                PostgresDB db;
                std::string conn = get_conn();
                db.connect(conn);
                db.execute(std::string("UPDATE users SET cards = array_remove(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                user->del_product(data);
            }
            else{
                auto user = find_user(id);
                auto ptr = TelegramSender::get_instance();
                ptr->call(std::string(id), type_msg::send, std::string("Неправильная команда"));
            }

            long long oset = std::stoll(offset);
            oset++;
            offset = std::to_string(oset);
            offset_reload(offset);
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

BotTelegram::BotTelegram(BotTelegram&& obj) noexcept
{
    offset = std::move(obj.offset);
    users = std::move(obj.users);
}

BotTelegram& BotTelegram::operator=(BotTelegram&& obj) noexcept
{
    if(this == &obj)
        return *this;

    offset = std::move(obj.offset);
    users = std::move(obj.users);
    return *this;
}

BotTelegram::~BotTelegram()
{
    stop_flag = true;
    std::string conn = get_conn();
    PostgresDB db;
    db.connect(conn);
    for(const auto& user : users){
        auto res = db.fetch(std::string("SELECT 1 FROM users WHERE id = $1 LIMIT 1"), std::vector<std::string>{user->get_id()});
        if(res.empty()){
            auto cards = user->get_cards();
            std::string card = "{";
            for(auto iter = cards.begin(); iter != cards.end(); iter++){
                card = card + *iter + ',';
            }
            card.back() = '}';
            db.execute(std::string("INSERT INTO users (id, cards) VALUES ($1, $2)"), std::vector<std::string>{user->get_id(), card});
        }
    }
}