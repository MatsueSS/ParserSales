#include <iostream>

#include "BotTelegram.h"
#include "json.hpp"

int main(void)
{
    BotTelegram bot("512291456");
    
    std::ifstream file("../res/cards.json");
    nlohmann::json json = nlohmann::json::parse(file);
    for(const auto& obj : json["cards"]){
        std::string name = obj["name"];
        bot.notify_all("3. "+name);
    }

    while(true){}

    return 0;
}