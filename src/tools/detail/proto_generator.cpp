#include "handler_template_file.h"
#include "proto_generator.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>

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

std::string celeritas::proto_generator::get_proto_full_name() const
{
    auto proto_full_name = message_full_name_;

    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    return proto_full_name;
}

