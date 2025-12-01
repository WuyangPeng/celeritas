#include "we_chat_sdk_process.h"

celeritas::we_chat_sdk_process::we_chat_sdk_process(sdk_process_parameter sdk_process_parameter)
    : base_type{ std::move(sdk_process_parameter) }
{
}

celeritas::sdk_process::optional_string_awaitable_type celeritas::we_chat_sdk_process::get_open_id()
{
    co_return std::nullopt;
}