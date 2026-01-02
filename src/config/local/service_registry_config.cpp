#include "service_registry_config.h"

celeritas::service_registry_config::service_registry_config(std::string name, std::string host, const int port) noexcept
    : name_{ std::move(name) }, host_{ std::move(host) }, port_{ port }
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

std::string celeritas::service_registry_config::get_name() const
{
    return name_;
}