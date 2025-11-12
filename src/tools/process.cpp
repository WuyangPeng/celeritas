#include "generate_handler.h"
#include "process.h"
#include "tools_fwd.h"
#include "common/celeritas_error.h"

celeritas::process::process_unique_ptr celeritas::process::create_process(const std::string_view process_name, std::string_view directory)
{
    if (process_name == process_generate_handler)
    {
        return std::make_unique<generate_handler>(directory);
    }

    throw celeritas_error("unable to create generate handler");
}