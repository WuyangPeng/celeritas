#include "generate_handler_file.h"
#include "common/logger.h"
#include "importer_error_collector.h"
#include "tools/tools_fwd.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <google/protobuf/compiler/importer.h>

#include <fstream>

using namespace std::literals;

celeritas::generate_handler_file::generate_handler_file(std::string proto_file,
                                                        std::string proto_directory,
                                                        std::string output_directory,
                                                        std::string template_directory,
                                                        const handler_template_file& handler_template_file)
    : proto_file_{ std::move(proto_file) },
      proto_directory_{ std::move(proto_directory) },
      output_directory_{ std::move(output_directory) },
      template_directory_{ std::move(template_directory) },
      handler_template_file_{ handler_template_file }
{
}

void celeritas::generate_handler_file::generate_file()
{
    LOG_CHANNEL(celeritas::default_channel, info) << "generate from file : " << proto_file_;

    google::protobuf::compiler::DiskSourceTree source_tree{};

    boost::filesystem::path path{ proto_directory_ };
    auto parent_path = path.parent_path().string();
    source_tree.MapPath("", parent_path);

    auto disk_path = proto_file_.substr(parent_path.length() + 1, proto_file_.length());
    // 1. 创建错误收集器实例
    importer_error_collector error_collector;
    google::protobuf::compiler::Importer importer{ &source_tree, &error_collector };
    std::ranges::replace(disk_path, '\\', '/');
    if (const auto* file_desc = importer.Import(disk_path))
    {
        for (auto i = 0; i < file_desc->message_type_count(); ++i)
        {
            const auto* message_desc = file_desc->message_type(i);

            for (auto j = 0; j < message_desc->oneof_decl_count(); ++j)
            {
                const auto* one_of_desc = message_desc->oneof_decl(j);

                field_type field{};
                for (auto k = 0; k < one_of_desc->field_count(); ++k)
                {
                    const auto* field_desc = one_of_desc->field(k);

                    auto field_name = field_desc->name();
                    auto field_type = field_desc->message_type()->name();

                    if (field_type.find(request_suffix) == std::string::npos &&
                        field_type.find(response_suffix) == std::string::npos)
                    {
                        break;
                    }

                    field.emplace(field_type, field_name);
                }
                if (!field.empty())
                {
                    const auto file_name = file_desc->name();
                    const auto message_full_name = message_desc->full_name();
                    const auto message_name = message_desc->name();
                    const auto one_of_name = one_of_desc->name();

                    generate_file(file_name, message_full_name, message_name, one_of_name, field);
                }
            }
        }
    }
    else
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "Failed to import proto file. Check errors above.";
    }
}

void celeritas::generate_handler_file::generate_file(std::string_view file_name, std::string_view message_full_name, std::string_view message_name, std::string_view one_of_name, const field_type& field)
{
    generate_header_file(file_name, message_full_name, message_name);
    generate_source_file(message_full_name, message_name, one_of_name, field);
}

void celeritas::generate_handler_file::generate_header_file(std::string_view file_name, const std::string_view message_full_name, const std::string_view message_name)
{
    auto message_handler_h_content = handler_template_file_.get_message_handler_h_content();

    std::string proto_path{ file_name };
    boost::replace_all(proto_path, ".proto", "");
    boost::replace_all(proto_path, ".", "/");

    std::string proto_full_name{ message_full_name };
    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    boost::replace_all(message_handler_h_content, "${proto_path}", proto_path);
    boost::replace_all(message_handler_h_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_h_content, "${proto_full_name}", proto_full_name);

    boost::filesystem::path path{ output_directory_ };
    path = path / (message_name.data() + "_message_handler.h"s);

    std::ofstream os(path.string(), std::ios::binary);

    os << message_handler_h_content;
}

void celeritas::generate_handler_file::generate_source_file(const std::string_view message_full_name, const std::string_view message_name, const std::string_view one_of_name, const field_type& field)
{
    auto message_handler_cpp_content = handler_template_file_.get_message_handler_cpp_content();

    std::string add_handler_function{};
    std::string payload_name{ one_of_name };
    if (!payload_name.empty())
    {
        payload_name[0] = std::toupper(payload_name[0]);
    }
    std::string proto_full_name{ message_full_name };
    boost::replace_all(proto_full_name, "celeritas.", "");
    boost::replace_all(proto_full_name, ".", "::");

    for (const auto& element : field)
    {
        std::string entry_enum_name{ element.second };
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

        auto add_handler_function_content = handler_template_file_.get_add_handler_function_content();

        boost::replace_all(add_handler_function_content, "${proto_name}", message_name);
        boost::replace_all(add_handler_function_content, "${proto_full_name}", proto_full_name);
        boost::replace_all(add_handler_function_content, "${payload_name}", payload_name);
        boost::replace_all(add_handler_function_content, "${entry_enum_name}", entry_enum_name);
        boost::replace_all(add_handler_function_content, "${entry_name}", element.second);

        add_handler_function += add_handler_function_content;
    }

    boost::replace_all(message_handler_cpp_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_cpp_content, "${add_handler_function}", add_handler_function);

    boost::filesystem::path path{ output_directory_ };
    path = path / (message_name.data() + "_message_handler.cpp"s);

    std::ofstream os(path.string(), std::ios::binary);

    os << message_handler_cpp_content;
}