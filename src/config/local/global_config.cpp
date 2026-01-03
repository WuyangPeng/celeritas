#include "global_config.h"

celeritas::global_config::global_config(std::string external_host) noexcept
    : external_host_{ std::move(external_host) }
{
}

std::string celeritas::global_config::get_external_host() const
{
    return external_host_;
}