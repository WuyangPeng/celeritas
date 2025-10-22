#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class player_server final : public server_base
    {
    public:
        using class_type = player_server;
        using base_type = server_base;

        player_server();
    };
}
