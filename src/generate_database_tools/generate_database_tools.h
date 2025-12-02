#pragma once

#include "../initializer/server_base.h"

namespace celeritas
{
    class generate_database_tools final : public celeritas_main
    {
    public:
        using class_type = generate_database_tools;
        using base_type = celeritas_main;

        generate_database_tools();

        void run(int argc, char** argv) const override;

    private:
        void create_initializer(const command_line_config& command_line_config) const override;
    };
}
