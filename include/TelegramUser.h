#ifndef _TELEGRAM_USER_H_
#define _TELEGRAM_USER_H_

#include <unordered_set>
#include <string>

class TelegramUser{
public:
    TelegramUser(const std::string& id);

    template<typename Type>
    void notify(Type&&) const;

    template<typename Type>
    void add_product(Type&&);

    template<typename Type>
    void del_product(Type&&);

private:
    std::string id;
    std::unordered_set<std::string> lovely_product;
};



#endif //_TELEGRAM_USER_H_