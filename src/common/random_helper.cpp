#include "celeritas_error.h"
#include "random_helper.h"

int celeritas::random_helper::get_random_int(const int end)
{
    return get_random_int(0, end);
}

int celeritas::random_helper::get_random_int(const int begin, const int end)
{
    if (begin >= end)
    {
        throw celeritas_error{ "get_random_int: begin ({}) cannot be greater than end ({})", begin, end };
    }

    std::uniform_int_distribution distribution{ begin, end - 1 };

    return distribution(get_engine());
}

float celeritas::random_helper::get_random_float()
{
    return get_random_float(0.0f, 1.0f);
}

float celeritas::random_helper::get_random_float(const float begin, const float end)
{
    if (begin > end)
    {
        throw celeritas_error{ "get_random_float: begin ({}) cannot be greater than end ({})", begin, end };
    }

    std::uniform_real_distribution distribution{ begin, end };

    return distribution(get_engine());
}

double celeritas::random_helper::get_random_double()
{
    return get_random_double(0.0, 1.0);
}

double celeritas::random_helper::get_random_double(const double begin, const double end)
{
    if (begin > end)
    {
        throw celeritas_error{ "get_random_double: begin ({}) cannot be greater than end ({})", begin, end };
    }

    std::uniform_real_distribution distribution{ begin, end };

    return distribution(get_engine());
}

bool celeritas::random_helper::get_random_bool(const double p)
{
    std::bernoulli_distribution distribution{ p };

    return distribution(get_engine());
}

double celeritas::random_helper::get_random_normal(const double mean, const double stddev)
{
    std::normal_distribution distribution{ mean, stddev };

    return distribution(get_engine());
}

std::mt19937& celeritas::random_helper::get_engine()
{
    thread_local std::mt19937 engine{ std::random_device{}() };

    return engine;
}