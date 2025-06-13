#include <iostream>
#include <fstream>

#include "TelegramSender.h"

int main(void)
{
    std::string temp, id, offset = "512291455", msg = "hello";
    std::ifstream file("../.env");
    getline(file, temp);
    getline(file, id);
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::read, offset);
}