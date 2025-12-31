#include "celeritas_main.h"
#include "common/core/command_line_config.h"

celeritas::celeritas_main::celeritas_main(const std::string_view& server_type)
    : server_type_{ server_type }
{
}

void celeritas::celeritas_main::run(const int argc, char** argv)
{
    if (const command_line_config command_line_config{ argc, argv, server_type_ };
        !command_line_config.is_exit_requested())
    {
        create_initializer(command_line_config);
    }
}

std::string celeritas::celeritas_main::get_server_type() const
{
    return server_type_;
}