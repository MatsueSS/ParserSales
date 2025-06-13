#include <iostream>
#include <fstream>

#include "JsonReader.h"

int main(void)
{
    std::ofstream file("../res/test_JsonReader.txt");
    auto res1 = JsonReader::read(std::string("jq -r '.date' ../res/cards.json"), type_json::products);
    for(const auto& obj : res1)
        file << obj << '\n';

    auto res2 = JsonReader::read(std::string("jq -r '.result[] | {text: .message.text}' ../res/result_512291454.json"), type_json::message);
    for(const auto& obj : res2)
        file << obj << '\n';

    return 0;
}