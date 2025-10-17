#pragma once

#include <random>

namespace celeritas
{
    class random_helper
    {
    public:
        using class_type = random_helper;

        [[nodiscard]] static int get_random_int(int end);

    private:
        [[nodiscard]] static std::mt19937& get_engine();
    };
}