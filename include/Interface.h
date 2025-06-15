#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "BotTelegram.h"

#include <memory>

class Interface{
public:
    Interface(std::string);

    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;

    bool start();

    template<typename type>
    void add_user(type&&);

private:
    std::unique_ptr<BotTelegram> bot;

    bool control_date();
};

template<typename type>
void Interface::add_user(type&& obj)
{
    bot->add_user(std::forward<type>(obj));
}

#endif //_INTERFACE_H_