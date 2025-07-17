#include <iostream>
#include <thread>

#include "Interface.h"

int main(void)
{
    std::string offset = read_offset();
    Interface interface(offset);

    while(true){
        interface.start();
        std::this_thread::sleep_for(std::chrono::seconds(1000));
    }

    return 0;
}