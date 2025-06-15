#include <iostream>
#include <thread>

#include "Interface.h"

int main(void)
{
    Interface interface("512291459");

    while(true){
        std::cout << interface.start() << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1000));
    }

    return 0;
}