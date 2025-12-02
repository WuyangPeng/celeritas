#pragma once

#include "../initializer/server_base.h"

namespace celeritas
{
    class game_server final : public server_base
    {
    public:
        using class_type = game_server;
        using base_type = server_base;

        game_server();
    };
}


