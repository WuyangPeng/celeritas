#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class auth_server final : public server_base
    {
    public:
        using class_type = auth_server;
        using base_type = server_base;

        auth_server();
    };
}



