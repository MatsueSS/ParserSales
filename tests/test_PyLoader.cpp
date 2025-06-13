#include <iostream>
#include <fstream>

#include "PyLoader.h"

int main(void)
{
    std::ofstream file("../res/test_PyLoader.txt");
    file << "Result of execute script: " << PyLoader::script_load("python3 ../py_scripts/example_for_test.py");

    return 0;
}