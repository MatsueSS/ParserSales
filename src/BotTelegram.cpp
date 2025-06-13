#include "BotTelegram.h"
#include "JsonReader.h"
#include "TelegramSender.h"

#include <thread>

BotTelegram::BotTelegram(std::string offset) : offset(offset)
{
    std::thread thr(&BotTelegram::check_msg, this);
    thr.detach();
}

void BotTelegram::check_msg()
{
    std::string id, data;
    while(true){
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