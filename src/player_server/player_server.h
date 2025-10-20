#pragma once

#include "common/common_fwd.h"

namespace celeritas
{
    class player_server
    {
    public:
        using class_type = player_server;

        static void run(int argc, char** argv);

    private:
        static void create_initializer(const command_line_config& command_line_config);
    };
}
