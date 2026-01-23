#include "sdk_process.h"
#include "we_chat_sdk_process.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "common/core/celeritas_error.h"

celeritas::sdk_process::sdk_process(sdk_process_parameter sdk_process_parameter)
    : sdk_process_parameter_{ std::move(sdk_process_parameter) }
{
}

celeritas::sdk_process::sdk_process_unique_ptr celeritas::sdk_process::create_sdk_process(sdk_process_parameter sdk_process_parameter)
{
    switch (const auto process_type = sdk_process_parameter.get_process_type())
    {
        case sdk_process_type::we_chat:
        {
            return std::make_unique<we_chat_sdk_process>(std::move(sdk_process_parameter));
        }
        default:
        {
            throw celeritas_error{ "unsupported sdk process type = {}", static_cast<int>(process_type) };
        }
    }
}