#include "source_proto_generator.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>

celeritas::source_proto_generator::source_proto_generator(const std::string_view message_full_name,
                                                          const std::string_view message_name,
                                                          const std::string_view one_of_name,
                                                          const field_type& field,
                                                          std::string output_directory,
                                                          const handler_template_file& handler_template_file)
    : base_type{ message_full_name, message_name, std::move(output_directory), handler_template_file }, one_of_name_{ one_of_name }, field_{ field }
{
}

void celeritas::source_proto_generator::execute()
{
    auto message_handler_cpp_content = get_message_handler_cpp_content();

    std::string add_handler_function{};
    std::string payload_name{ one_of_name_ };
    if (!payload_name.empty())
    {
        payload_name[0] = std::toupper(payload_name[0]);
    }
    std::string proto_full_name{ get_message_full_name() };
    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    auto message_name = get_message_name();

    for (const auto& element : field_)
    {
        std::string entry_enum_name{ element };
        std::vector<std::string> parts;

        // 1. 按下划线分割字符串
        boost::split(parts, entry_enum_name, boost::is_any_of("_"));

        // 2. 遍历每个部分，将首字母转为大写
        for (std::string& part : parts)
        {
            if (!part.empty())
            {
                part[0] = std::toupper(part[0]);
            }
        }

        entry_enum_name = boost::join(parts, "");

        auto add_handler_function_content = get_add_handler_function_content();

        boost::replace_all(add_handler_function_content, "${proto_name}", message_name);
        boost::replace_all(add_handler_function_content, "${proto_full_name}", proto_full_name);
        boost::replace_all(add_handler_function_content, "${payload_name}", payload_name);
        boost::replace_all(add_handler_function_content, "${entry_enum_name}", entry_enum_name);
        boost::replace_all(add_handler_function_content, "${entry_name}", element);

        add_handler_function += add_handler_function_content;
    }

    boost::replace_all(message_handler_cpp_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_cpp_content, "${add_handler_function}", add_handler_function);

    boost::filesystem::path path{ get_output_directory() };
    path = path / (message_name + "_message_handler.cpp");

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

    if (existing_content == message_handler_cpp_content)
    {
        LOG_CHANNEL(default_channel, debug) << "File " << path << " content unchanged, skipping write.";
        return;
    }
    is.close();
    std::ofstream os(path.string(), std::ios::binary);

    os << message_handler_cpp_content;

    LOG_CHANNEL(celeritas::default_channel, info) << "generate file : " << path;
}