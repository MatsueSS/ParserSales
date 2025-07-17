#ifndef _BOT_TELEGRAM_H_
#define _BOT_TELEGRAM_H_

#include "TelegramUser.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <atomic>
#include <iostream>

class BotTelegram{
public:
    BotTelegram(std::string);

    BotTelegram(const BotTelegram&) = delete;
    BotTelegram& operator=(const BotTelegram&) = delete;

    BotTelegram(BotTelegram&&) noexcept;
    BotTelegram& operator=(BotTelegram&&) noexcept; 

    template<typename Type>
    void notify_all(Type&&);

    //in parall_thread for check message in telegram
    void check_msg();

    //only telegram_user
    void add_user(std::unique_ptr<TelegramUser>&&);

    //polynomial_search
    template<typename Type>
    TelegramUser* find_user(Type&&);

    ~BotTelegram();

private:
    std::string offset;
    std::unordered_map<std::string, std::unique_ptr<TelegramUser>> users;
    std::atomic<bool> stop_flag{false};

};

template<typename Type>
void BotTelegram::notify_all(Type&& sale)
{
    for(auto user = users.begin(); user != users.end(); user++){
        if(user->second->is_has(sale)){
            user->second->notify(std::forward<Type>(sale));
        }
    }
}

template<typename Type>
TelegramUser* BotTelegram::find_user(Type&& id)
{
    return users.find(id) != users.end() ? users.find(id)->second.get() : nullptr;
}

#endif //_BOT_TELEGRAM_H_