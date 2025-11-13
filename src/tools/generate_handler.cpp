#include "generate_handler.h"
#include "importer_error_collector.h"
#include "tools_fwd.h"
#include "boost/filesystem/path.hpp"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/command_line_config.tpp"

#include <google/protobuf/compiler/importer.h>

#include <filesystem>
#include <fstream>

celeritas::generate_handler::generate_handler(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      proto_directory_{ command_line_config.get<std::string>(proto_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) }
{
    if (!std::filesystem::exists(proto_directory_) || !std::filesystem::is_directory(proto_directory_))
    {
        throw celeritas_error(proto_directory_ + " is no directory found.");
    }

    if (!std::filesystem::exists(output_directory_) || !std::filesystem::is_directory(output_directory_))
    {
        throw celeritas_error(output_directory_ + " is no directory found.");
    }
}

void celeritas::generate_handler::execute()
{
    try
    {
        generate(proto_directory_);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_handler::generate(const std::string& directory)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            if (const auto& file_path = entry.path();
                file_path.extension() == ".proto")
            {
                generate_file(file_path.string());
            }
        }
        else if (entry.is_directory())
        {
            generate(entry.path().string());
        }
    }
}

void celeritas::generate_handler::generate_file(const std::string& proto_file)
{
    LOG_CHANNEL(celeritas::default_channel, info) << "generate from file : " << proto_file;

    google::protobuf::compiler::DiskSourceTree source_tree;

    boost::filesystem::path path{ proto_directory_ };
    auto parent_path = path.parent_path().string();
    source_tree.MapPath("", parent_path);

    auto disk_path = proto_file.substr(parent_path.length() + 1, proto_file.length());
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

                for (auto k = 0; k < one_of_desc->field_count(); ++k)
                {
                    const auto* field_desc = one_of_desc->field(k);

                    auto field_name = field_desc->name();
                    auto field_full_type = field_desc->message_type() ?
                                               field_desc->message_type()->full_name() :
                                               "Unknown Type";
                }
            }
        }
    }
    else
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "Failed to import proto file. Check errors above.";
    }
}
