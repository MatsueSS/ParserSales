#include "TelegramUser.h"

TelegramUser::TelegramUser(const std::string& id) noexcept : id(id) {}

std::string TelegramUser::get_id() const noexcept
{
    return id;
}

TelegramUser::TelegramUser(TelegramUser&& obj) noexcept
{
    lovely_product = std::move(obj.lovely_product);
    id = std::move(obj.id);
}

TelegramUser& TelegramUser::operator=(TelegramUser&& obj) noexcept
{
    if(this == &obj)
        return *this;

    lovely_product = std::move(obj.lovely_product);
    id = std::move(obj.id);
    return *this;
}

const std::unordered_set<std::string>& TelegramUser::get_cards() const noexcept
{
    return lovely_product;
}

bool TelegramUser::operator==(const TelegramUser& obj) const noexcept
{
    return id == obj.id;
}

int TelegramUser::count_cards() const noexcept
{
    return lovely_product.size();
}

TUexception::TUexception(std::string str) : msg(std::move(str)) {}

TUexception::TUexception(const TUexception& obj) : msg(obj.msg) {}

BadTypeValueTUexception::BadTypeValueTUexception(std::string str) : TUexception(std::move(str)) {}

const char * TUexception::what() const noexcept 
{
    return msg.c_str();
}

EmptyQueryResultTUexception::EmptyQueryResultTUexception(std::string str) : TUexception(std::move(str)) {}