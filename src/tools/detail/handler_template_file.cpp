#include "handler_template_file.h"
#include "common/logger.h"
#include "tools/tools_fwd.h"

#include <boost/filesystem/path.hpp>

#include <fstream>

celeritas::handler_template_file::handler_template_file(std::string template_directory)
    : template_directory_{ std::move(template_directory) }, message_handler_h_content_{}, message_handler_cpp_content_{}, add_handler_function_content_{}
{
    load_template();
}

std::string celeritas::handler_template_file::get_message_handler_h_content() const
{
    return message_handler_h_content_;
}

std::string celeritas::handler_template_file::get_message_handler_cpp_content() const
{
    return message_handler_cpp_content_;
}

std::string celeritas::handler_template_file::get_add_handler_function_content() const
{
    return add_handler_function_content_;
}

void celeritas::handler_template_file::load_template()
{
    load_template(message_handler_h_template_name, message_handler_h_content_);
    load_template(message_handler_cpp_template_name, message_handler_cpp_content_);
    load_template(add_handler_function_template_name, add_handler_function_content_);
}

void celeritas::handler_template_file::load_template(const std::string_view template_name, std::string& content) const
{
    boost::filesystem::path path{ template_directory_ };
    const auto file_name = path / template_name;

    std::ifstream is(file_name, std::ios::binary);
    if (!is)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "Failed to load" << template_name;
    }

    content.assign((std::istreambuf_iterator{ is }), (std::istreambuf_iterator<char>()));
}