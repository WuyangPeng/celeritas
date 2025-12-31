#include "server_base.h"
#include "common/core/command_line_config.h"
#include "initializer/initializer.h"

celeritas::server_base::server_base(const std::string_view& server_type)
    : base_type{ server_type }
{
}

void celeritas::server_base::create_initializer(const command_line_config& command_line_config)
{
    const auto server_context = initializer::create(get_server_type(), command_line_config.get<std::string>(config_file_path_command_line.data()));

    server_context->initialize();
    server_context->run();
}