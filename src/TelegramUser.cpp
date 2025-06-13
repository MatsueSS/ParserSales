#include "TelegramUser.h"

TelegramUser::TelegramUser(const std::string& id) : id(id) {}

std::string TelegramUser::get_id() const
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

const std::unordered_set<std::string>& TelegramUser::get_cards() const
{
    return lovely_product;
}