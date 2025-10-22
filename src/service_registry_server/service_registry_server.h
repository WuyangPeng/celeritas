#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class service_registry_server final : public server_base
    {
    public:
        using class_type = service_registry_server;
        using base_type = server_base;

        service_registry_server();
    };
}
