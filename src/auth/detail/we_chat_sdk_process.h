#pragma once

#include "sdk_process.h"

namespace celeritas
{
    class we_chat_sdk_process : public sdk_process
    {
    public:
        using class_type = we_chat_sdk_process;
        using base_type = sdk_process;

        we_chat_sdk_process(sdk_process_parameter sdk_process_parameter);

        [[nodiscard]] optional_string_awaitable_type get_open_id() override;
    };
}