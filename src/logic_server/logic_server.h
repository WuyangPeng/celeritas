#pragma once

#include "initializer/server_base.h"

namespace celeritas
{
    class logic_server final : public server_base
    {
    public:
        using class_type = logic_server;
        using base_type = server_base;

        logic_server();
    };
}
