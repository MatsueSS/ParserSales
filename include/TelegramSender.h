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
    static TelegramSender& get_instance();

    TelegramSender(const TelegramSender&) = delete;
    TelegramSender& operator=(const TelegramSender&) = delete;

    //need id, type_msg, and offset
    template<typename Data, typename Type>
    static std::future<void> call(Data&& id, Type&& type, Data&& offset);

private:
    std::string token;

    TelegramSender();

    template<typename Data, typename Type>
    void query(Data&& id, Type&& type, Data&& offset);
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

template<typename Data, typename Type>
void TelegramSender::query(Data&& id, Type&& type, Data&& offset)
{
    std::string url = "https://api.telegram.org/bot" + token;
    switch (type)
    {
    case type_msg::read:
    {
        url = url + "/getUpdates?offset=" + offset;
        std::ofstream file("..res/result_" + offset + ".json");
        CURL* curl = curl_easy_init();
        if(curl){
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        break;
    }
    case type_msg::send:
    {
        url = url + "/sendMessage";
        CURL* curl = curl_easy_init();
        if(curl){
            char* encoding = curl_easy_escape(curl, offset.c_str(), offset.length());
            std::string post_fields = "chat_id="+id+"&text="+encoding;
            curl_free(encoding);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        break;
    }
    default:
        break;
    }
}

#endif //_TELEGRAM_SENDER_H_