#pragma once

#include "common/common_fwd.h"

namespace celeritas
{
    class gateway_server
    {
    public:
        using class_type = gateway_server;

        static void run(int argc, char** argv);

    private:
        static void create_initializer(const command_line_config& command_line_config);
    };
}
