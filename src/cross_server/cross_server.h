#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class cross_server final : public server_base
    {
    public:
        using class_type = cross_server;
        using base_type = server_base;

        cross_server();
    };
}


