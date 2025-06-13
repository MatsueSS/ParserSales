#ifndef _TELEGRAM_SENDER_H_
#define _TELEGRAM_SENDER_H_

//constructor read a .env file in fisrt row need token
//The following parameters are supplied for the method call:
//id, type_msg, offset msg

#include <memory>
#include <future>
#include <curl/curl.h>

enum class type_msg{
    read, send
};

class TelegramSender{
public:
    static TelegramSender* get_instance();

    TelegramSender(const TelegramSender&) = delete;
    TelegramSender& operator=(const TelegramSender&) = delete;

    //need id, type_msg, and offset
    template<typename Data, typename Type>
    static std::future<void> call(Data&& id, Type&& type, Data&& offset);

private:
    static TelegramSender* instance;
    std::string token;

    TelegramSender();

    void query(std::string id, type_msg type, std::string offset);
};

size_t writeCallback(void*, size_t, size_t, void*);

template<typename Data, typename Type>
std::future<void> TelegramSender::call(Data&& id, Type&&type, Data&& offset)
{
    return std::async(std::launch::async, &TelegramSender::query, 
        get_instance(),
        std::forward<Data>(id),
        std::forward<Type>(type),
        std::forward<Data>(offset)
    );
}

#endif //_TELEGRAM_SENDER_H_