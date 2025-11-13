#include "handler_template_file.h"
#include "proto_generator.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>

#include <fstream>

celeritas::proto_generator::proto_generator(const std::string_view message_full_name,
                                            const std::string_view message_name,
                                            std::string output_directory,
                                            const handler_template_file& handler_template_file)
    : message_full_name_{ message_full_name },
      message_name_{ message_name },
      output_directory_{ std::move(output_directory) },
      handler_template_file_{ handler_template_file }
{
}

std::string celeritas::proto_generator::get_message_name() const
{
    return message_name_;
}

std::string celeritas::proto_generator::get_message_full_name() const
{
    return message_full_name_;
}

std::string celeritas::proto_generator::get_output_directory() const
{
    return output_directory_;
}

std::string celeritas::proto_generator::get_message_handler_h_content() const
{
    return handler_template_file_.get_message_handler_h_content();
}

std::string celeritas::proto_generator::get_message_handler_cpp_content() const
{
    return handler_template_file_.get_message_handler_cpp_content();
}

std::string celeritas::proto_generator::get_add_handler_function_content() const
{
    return handler_template_file_.get_add_handler_function_content();
}

bool celeritas::proto_generator::is_content_same(const std::string& file_name, const std::string& content)
{
    if (std::ifstream stream{ file_name, std::ios::binary };
        stream.good())
    {
        std::string existing_content{};
        existing_content.assign(
            (std::istreambuf_iterator{ stream }),
            (std::istreambuf_iterator<char>())
            );

        if (existing_content == content)
        {
            LOG_CHANNEL(default_channel, debug) << "File " << file_name << " content unchanged, skipping write.";
            return true;
        }
    }

    return false;
}

void celeritas::proto_generator::save_handler(const std::string& file_name, const std::string& content)
{
    std::ofstream os{ file_name, std::ios::binary };

    os << content;

    LOG_CHANNEL(celeritas::default_channel, info) << "generate file : " << file_name;
}

std::string celeritas::proto_generator::get_proto_full_name() const
{
    auto proto_full_name = message_full_name_;

    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    return proto_full_name;
}

