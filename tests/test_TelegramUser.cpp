#include <iostream>
#include <fstream>

#include "TelegramUser.h"

int main(void)
{
    std::ifstream file("../.env");
    std::string temp, id;
    getline(file, temp);
    getline(file, id);

    TelegramUser user(id);
    user.add_product(std::string("Mollis"));
    user.notify(std::string("3. Mollis"));

    return 0;
}