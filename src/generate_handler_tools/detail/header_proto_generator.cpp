#include "header_proto_generator.h"
#include "generate_handler_tools/generate_handler_tools_fwd.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>

celeritas::header_proto_generator::header_proto_generator(const std::string_view file_name,
                                                          const std::string_view message_full_name,
                                                          const std::string_view message_name,
                                                          std::string output_directory,
                                                          const handler_template_file& handler_template_file)
    : base_type{ message_full_name, message_name, std::move(output_directory), handler_template_file },
      file_name_{ file_name },
      proto_path_{ get_proto_path(file_name_) }
{
}

void celeritas::header_proto_generator::execute()
{
    auto message_handler_h_content = get_message_handler_h_content();

    const auto proto_full_name = get_proto_full_name();
    const auto message_name = get_message_name();

    boost::replace_all(message_handler_h_content, "${proto_path}", proto_path_);
    boost::replace_all(message_handler_h_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_h_content, "${proto_full_name}", proto_full_name);

    boost::filesystem::path path{ get_output_directory() };
    path = path / (message_name + "_message_handler.h");

    if (const auto file_name = path.string();
        !is_content_same(file_name, message_handler_h_content))
    {
        save_handler(file_name, message_handler_h_content);
    }
}

std::string celeritas::header_proto_generator::get_proto_path(const std::string& file_name)
{
    auto proto_path = file_name;
    boost::replace_all(proto_path, proto_extension, "");
    boost::replace_all(proto_path, ".", "/");

    return proto_path;
}

