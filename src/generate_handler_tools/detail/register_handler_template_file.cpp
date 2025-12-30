#include "register_handler_template_file.h"
#include "../../common/core/celeritas_error.h"
#include "generate_handler_tools/generate_handler_tools_fwd.h"

#include <filesystem>
#include <fstream>

using namespace std::literals;

celeritas::register_handler_template_file::register_handler_template_file(std::string template_directory)
    : template_directory_{ std::move(template_directory) }, register_handler_helper_cpp_content_{}
{
    load_template();
}

std::string celeritas::register_handler_template_file::get_register_handler_helper_cpp_content() const
{
    return register_handler_helper_cpp_content_;
}

void celeritas::register_handler_template_file::load_template()
{
    const std::filesystem::path path{ template_directory_ };
    const auto file_name = path / register_handler_helper_template_name;

    std::ifstream is{ file_name.string(), std::ios::binary };
    if (!is)
    {
        throw celeritas_error("failed to load"s + register_handler_helper_template_name.data());
    }

    register_handler_helper_cpp_content_.assign((std::istreambuf_iterator{ is }), (std::istreambuf_iterator<char>()));
}