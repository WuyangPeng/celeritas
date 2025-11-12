#include "generate_handler.h"
#include "process.h"
#include "tools_fwd.h"
#include "common/celeritas_error.h"

celeritas::process::process(command_line_config command_line_config)
    : command_line_config_{ std::move(command_line_config) }
{
}

celeritas::process::process_unique_ptr celeritas::process::create_process(const command_line_config& command_line_config)
{
    const auto process_name = command_line_config.get<std::string>(process_command_line.data());
    if (process_name == process_generate_handler)
    {
        return std::make_unique<generate_handler>(command_line_config);
    }

    throw celeritas_error("unable to create generate handler");
}

