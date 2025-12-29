#include "generate_database.h"
#include "generate_mysql.h"
#include "generate_database_tools_fwd.h"
#include "process.h"
#include "../common/core_utilities/celeritas_error.h"

#include <filesystem>

celeritas::process::process(command_line_config command_line_config)
    : command_line_config_{ std::move(command_line_config) }
{
}

celeritas::process::process_unique_ptr celeritas::process::create_process(const command_line_config& command_line_config)
{
    const auto process_name = command_line_config.get<std::string>(process_command_line.data());
    if (process_name == process_generate_database)
    {
        return std::make_unique<generate_database>(command_line_config);
    }
    else if (process_name == process_generate_mysql)
    {
        return std::make_unique<generate_mysql>(command_line_config);
    }

    throw celeritas_error("unable to create generate database");
}

void celeritas::process::check_directory_exists(const std::string& directory)
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
    {
        throw celeritas_error(directory + " is no directory found.");
    }
}
