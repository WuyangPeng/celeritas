#include "random_helper.h"

int celeritas::random_helper::get_random_int(const int end)
{
    std::uniform_int_distribution<> distribution{ 0, end - 1 };

    return distribution(get_engine());
}

std::mt19937& celeritas::random_helper::get_engine()
{
    static std::mt19937 engine{ std::random_device{}() };

    return engine;
}