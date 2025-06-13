#include "BotTelegram.h"
#include "JsonReader.h"
#include "TelegramSender.h"
#include "PostgresDB.h"

#include "good_function.hpp"

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
        users.emplace_back(std::move(user));
    }
}

void BotTelegram::check_msg()
{
    std::string id, data;
    while(!stop_flag.load()){
        std::this_thread::sleep_for(std::chrono::seconds(10));
        auto ptr = TelegramSender::get_instance();
        ptr->call(std::string(""), type_msg::read, std::string(offset));
        auto v = JsonReader::read("jq -r '.result[] | select(.message.text != null) | {text: .message.text, id: .message.from.id}' ../res/result_"+ offset +".json", type_json::message);
        if(!v.empty())
        {
            id = v[2];
            data = v[1];

            if(data == "start"){
                users.emplace_back(std::make_unique<TelegramUser>(id));
            }
            else{
                for(const auto& user : users){
                    if(user->get_id() == id){
                        user->notify(data);
                        break;
                    }
                }
            }

            long long oset = std::stoll(offset);
            oset++;
            offset = std::to_string(oset);
        }
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