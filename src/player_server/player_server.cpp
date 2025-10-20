#include "player_server.h"
#include "common/command_line_config.tpp"
#include "common/logger.h"
#include "initializer/initializer.h"
#include "server/server_fwd.h"

#include <exception>

void celeritas::player_server::run(const int argc, char** argv)
{
    if (const command_line_config command_line_config{ argc, argv, player_type };
        !command_line_config.is_exit_requested())
    {
        create_initializer(command_line_config);
    }
}

void celeritas::player_server::create_initializer(const command_line_config& command_line_config)
{
    const auto server_context = initializer::create(player_type, command_line_config.get<std::string>(config_file_path_command_line.data()));

    server_context->initialize();
    server_context->run();
}

int main(const int argc, char** argv)
{
    try
    {
        celeritas::player_server::run(argc, argv);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error:" << error.what();
        return 1;
    }
    catch (...)
    {
        LOG_CHANNEL(celeritas::default_channel, fatal) << "unknown error.";
        return 1;
    }

    return 0;
}