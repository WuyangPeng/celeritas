#include "generate_database_tools.h"
#include "generate_database_tools_fwd.h"
#include "process.h"
#include "common/command_line_config.tpp"
#include "common/logger.h"
#include "server/server_fwd.h"

#include <exception>

celeritas::generate_database_tools::generate_database_tools()
    : base_type{ generate_database_tools_type }
{
}

void celeritas::generate_database_tools::run(const int argc, char** argv) const
{
    if (const command_line_config command_line_config{ argc,
                                                       argv,
                                                       get_server_type(),
                                                       { { process_command_line.data(), process_description.data() },
                                                         { database_directory_command_line.data(), database_directory_description.data() },
                                                         { output_directory_command_line.data(), output_directory_description.data() },
                                                         { template_directory_command_line.data(), template_directory_description.data() } } };
        !command_line_config.is_exit_requested())
    {
        create_initializer(command_line_config);
    }
}

void celeritas::generate_database_tools::create_initializer(const command_line_config& command_line_config) const
{
    const auto process_unique_ptr = process::create_process(command_line_config);
    process_unique_ptr->execute();
}

int main(const int argc, char** argv)
{
    try
    {
        const celeritas::generate_database_tools tools{};

        tools.run(argc, argv);
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

