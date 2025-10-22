#include "server_base.h"
#include "common/command_line_config.tpp"
#include "initializer/initializer.h"

celeritas::server_base::server_base(const std::string_view& server_type)
    : server_type_{ server_type }
{
}

void celeritas::server_base::run(int argc, char** argv)
{
    if (const command_line_config command_line_config{ argc, argv, server_type_ };
        !command_line_config.is_exit_requested())
    {
        create_initializer(command_line_config);
    }
}

void celeritas::server_base::create_initializer(const command_line_config& command_line_config)
{
    const auto server_context = initializer::create(server_type_, command_line_config.get<std::string>(config_file_path_command_line.data()));

    server_context->initialize();
    server_context->run();
}