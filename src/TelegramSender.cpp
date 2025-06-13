#include "TelegramSender.h"

#include <fstream>

TelegramSender::TelegramSender()
{
    std::ifstream file("../.env");
    curl_global_init(CURL_GLOBAL_DEFAULT);
    file >> token;
}

TelegramSender& TelegramSender::get_instance()
{
    static TelegramSender instance;
    return instance;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::ofstream* response = static_cast<std::ofstream*>(userp);
    size_t total_size = size*nmemb;
    response->write(static_cast<char*>(contents), total_size);
    return total_size;
}
