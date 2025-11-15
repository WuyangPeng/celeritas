#include "protobuf_compiler_importer.h"

#include <boost/filesystem/operations.hpp>

#include <algorithm>

celeritas::protobuf_compiler_importer::protobuf_compiler_importer(std::string proto_file, std::string proto_directory)
    : proto_file_{ std::move(proto_file) },
      proto_directory_{ std::move(proto_directory) },
      current_path_{ proto_directory_ },
      parent_path_{ current_path_.parent_path() },
      disk_path_{ get_disk_path(proto_file_, parent_path_) },
      error_collector_{},
      disk_source_tree_{ get_disk_source_tree(parent_path_) },
      compiler_importer_{ disk_source_tree_.get(), &error_collector_ }
{
}

celeritas::protobuf_compiler_importer::file_descriptor_type celeritas::protobuf_compiler_importer::get_file_descriptor()
{
    return compiler_importer_.Import(disk_path_);
}

std::string celeritas::protobuf_compiler_importer::get_disk_path(const std::string& proto_file, const path_type& parent_path)
{
    const auto disk_path_boost = boost::filesystem::relative(proto_file, parent_path);

    auto disk_path = disk_path_boost.string();

    std::ranges::replace(disk_path, '\\', '/');

    return disk_path;
}

celeritas::protobuf_compiler_importer::disk_source_tree_unique_ptr celeritas::protobuf_compiler_importer::get_disk_source_tree(const path_type& parent_path)
{
    auto disk_source_tree = std::make_unique<disk_source_tree_type>();

    disk_source_tree->MapPath("", parent_path.string());

    return disk_source_tree;
}