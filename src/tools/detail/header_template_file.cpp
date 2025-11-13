#include "header_template_file.h"
#include "common/celeritas_error.h"
#include "tools/tools_fwd.h"

#include <boost/filesystem/path.hpp>

#include <fstream>

using namespace std::literals;

celeritas::header_template_file::header_template_file(std::string template_directory)
    : template_directory_{ std::move(template_directory) }, handler_generated_h_content_{}
{
    load_template();
}

std::string celeritas::header_template_file::get_handler_generated_h_content() const
{
    return handler_generated_h_content_;
}

void celeritas::header_template_file::load_template()
{
    const boost::filesystem::path path{ template_directory_ };
    const auto file_name = path / handler_generated_template_name;

    std::ifstream is{ file_name, std::ios::binary };
    if (!is)
    {
        throw celeritas_error("failed to load"s + handler_generated_template_name.data());
    }

    handler_generated_h_content_.assign((std::istreambuf_iterator{ is }), (std::istreambuf_iterator<char>()));
}