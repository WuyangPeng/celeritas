#pragma once

#include "initializer/server/server_base.h"

namespace celeritas
{
    class payment_server final : public server_base
    {
    public:
        using class_type = payment_server;
        using base_type = server_base;

        payment_server();
    };
}
