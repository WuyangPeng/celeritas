#include "header_proto_generator.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>

celeritas::header_proto_generator::header_proto_generator(const std::string_view file_name,
                                                          const std::string_view message_full_name,
                                                          const std::string_view message_name,
                                                          std::string output_directory,
                                                          const handler_template_file& handler_template_file)
    : base_type{ message_full_name, message_name, std::move(output_directory), handler_template_file }, file_name_{ file_name }
{
}

void celeritas::header_proto_generator::execute()
{
    auto message_handler_h_content = get_message_handler_h_content();

    std::string proto_path{ file_name_ };
    boost::replace_all(proto_path, ".proto", "");
    boost::replace_all(proto_path, ".", "/");

    std::string proto_full_name{ get_message_full_name() };
    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    auto message_name = get_message_name();

    boost::replace_all(message_handler_h_content, "${proto_path}", proto_path);
    boost::replace_all(message_handler_h_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_h_content, "${proto_full_name}", proto_full_name);

    boost::filesystem::path path{ get_output_directory() };
    path = path / (message_name + "_message_handler.h");

    std::ifstream is(path, std::ios::binary);
    std::string existing_content;
    if (is.good())
    {
        // 使用高效的迭代器方式读取整个文件内容
        existing_content.assign(
            (std::istreambuf_iterator<char>(is)),
            (std::istreambuf_iterator<char>())
            );
        is.close();
    }

    if (existing_content == message_handler_h_content)
    {
        LOG_CHANNEL(default_channel, debug) << "File " << path << " content unchanged, skipping write.";
        return;
    }
    is.close();
    std::ofstream os(path.string(), std::ios::binary);

    os << message_handler_h_content;

    LOG_CHANNEL(celeritas::default_channel, info) << "generate file : " << path;
}