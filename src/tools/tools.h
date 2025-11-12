#pragma once

#include "server/server_base.h"

namespace celeritas
{
    class tools final : public celeritas_main
    {
    public:
        using class_type = tools;
        using base_type = celeritas_main;

        tools();

        void run(int argc, char** argv) const override;

    private:
        void create_initializer(const command_line_config& command_line_config) const;
    };
}
