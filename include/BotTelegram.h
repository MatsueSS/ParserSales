#ifndef _BOT_TELEGRAM_H_
#define _BOT_TELEGRAM_H_

#include "TelegramUser.h"

#include <string>
#include <vector>
#include <memory>
#include <fstream>

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

    //only telegtam_user
    template<typename Type>
    void add_user(Type&&);

    //polynomial_search
    template<typename Type>
    TelegramUser* find_user(Type&&);

private:
    std::string offset;
    std::vector<std::unique_ptr<TelegramUser>> users;

};

template<typename Type>
void BotTelegram::notify_all(Type&& sale)
{
    for(const auto& user : users)
        user->notify(std::forward<Type>(sale));
}

template<typename Type>
void BotTelegram::add_user(Type&& user)
{
    users.emplace_back(user);
}

template<typename Type>
TelegramUser* BotTelegram::find_user(Type&& id)
{
    auto it = std::find_if(users.begin(), users.end(), [&](const std::unique_ptr<TelegramUser>& obj){return obj->get_id() == id;});
    return it->get();
}

#endif //_BOT_TELEGRAM_H_