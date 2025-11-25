#pragma once

#include "command_line_config.h"
#include "common/celeritas_error.h"

template <typename T>
T celeritas::command_line_config::get(const std::string& key) const
{
    if (variables_.contains(key))
    {
        return variables_[key].as<T>();
    }

    throw celeritas_error("attempted to access unregistered or invalid key: {}", key);
}