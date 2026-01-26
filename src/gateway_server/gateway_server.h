#pragma once

#include "initializer/server/server_base.h"

namespace celeritas
{
    class gateway_server final : public server_base
    {
    public:
        using class_type = gateway_server;
        using base_type = server_base;

        gateway_server();
    };
}

