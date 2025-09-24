#include "service_registry_config.h"

celeritas::service_registry_config::service_registry_config(std::string host, int port) noexcept
    : host_{ std::move(host) }, port_{ port }
{
}