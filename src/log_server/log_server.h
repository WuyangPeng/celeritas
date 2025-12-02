#pragma once

#include "initializer/server_base.h"

namespace celeritas
{
    class log_server final : public server_base
    {
    public:
        using class_type = log_server;
        using base_type = server_base;

        log_server();
    };
}




