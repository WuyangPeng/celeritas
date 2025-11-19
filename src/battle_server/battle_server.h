#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class battle_server final : public server_base
    {
    public:
        using class_type = battle_server;
        using base_type = server_base;

        battle_server();
    };
}