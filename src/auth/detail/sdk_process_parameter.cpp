#include "sdk_process_parameter.h"

celeritas::sdk_process_parameter::sdk_process_parameter(std::string sdk_token, sdk_providers sdk_providers)
    : sdk_token_{ std::move(sdk_token) }, sdk_providers_{ std::move(sdk_providers) }
{
}

std::string celeritas::sdk_process_parameter::get_sdk_token() const
{
    return sdk_token_;
}

celeritas::sdk_providers celeritas::sdk_process_parameter::get_sdk_providers() const
{
    return sdk_providers_;
}

celeritas::sdk_process_type celeritas::sdk_process_parameter::get_process_type() const
{
    return static_cast<sdk_process_type>(sdk_providers_.get_process_type());
}