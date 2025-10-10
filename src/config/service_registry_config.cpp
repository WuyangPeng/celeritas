#include "service_registry_config.h"

celeritas::service_registry_config::service_registry_config(std::string host, int port) noexcept
    : host_{ std::move(host) }, port_{ port }
{
}

std::string celeritas::service_registry_config::get_host() const
{
    return host_;
}

int celeritas::service_registry_config::get_port() const noexcept
{
    return port_;
}