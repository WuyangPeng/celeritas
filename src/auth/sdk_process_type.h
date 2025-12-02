#pragma once

#include <string>

namespace celeritas
{
    enum class sdk_process_type
    {
        null = 0,
        we_chat = 1,
    };

    [[nodiscard]] std::string get_sdk_process_type_description(sdk_process_type sdk_process_type);
}