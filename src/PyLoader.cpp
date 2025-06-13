#include "PyLoader.h"

#include <cstdlib>

int PyLoader::script_load(std::string query)
{
    return WEXITSTATUS(system(query.c_str()));
}