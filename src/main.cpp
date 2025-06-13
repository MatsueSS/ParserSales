#include <iostream>

#include "BotTelegram.h"

int main(void)
{
    BotTelegram bot("512291455");

    auto user = bot.find_user("828404782");

    return 0;
}