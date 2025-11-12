#include "generate_handler.h"
#include "common/celeritas_error.h"
#include "common/logger.h"

#include <filesystem>

using namespace std::literals;

celeritas::generate_handler::generate_handler(const std::string_view directory)
    : directory_{ directory }
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
    {
        throw celeritas_error(directory.data() + " is no directory found."s);
    }
}

void celeritas::generate_handler::execute()
{
}