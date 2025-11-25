#include "time_helper.h"

int64_t celeritas::time_helper::get_current_milliseconds()
{
    const auto now = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t celeritas::time_helper::get_current_seconds()
{
    const auto now = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}