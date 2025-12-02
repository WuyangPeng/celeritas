#pragma once

#include "initializer/server_base.h"

namespace celeritas
{
    class admin_server final : public server_base
    {
    public:
        using class_type = admin_server;
        using base_type = server_base;

        admin_server();
    };
}




