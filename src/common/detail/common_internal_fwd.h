#pragma once

#include <string>

namespace celeritas
{
    class logger_impl;

    constexpr std::string_view default_channel_{ "default" };
    constexpr std::string_view unregistered_channel_{ "unregistered" };
}
