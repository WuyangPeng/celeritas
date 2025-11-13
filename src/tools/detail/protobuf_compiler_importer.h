#pragma once

#include "importer_error_collector.h"

#include <boost/filesystem/path.hpp>

#include <string>

namespace celeritas
{
    class protobuf_compiler_importer
    {
    public:
        using class_type = protobuf_compiler_importer;
        using file_descriptor_type = const google::protobuf::FileDescriptor*;

        protobuf_compiler_importer(std::string proto_file, std::string proto_directory);

        [[nodiscard]] file_descriptor_type get_file_descriptor();

    private:
        using path_type = boost::filesystem::path;
        using disk_source_tree_type = google::protobuf::compiler::DiskSourceTree;
        using disk_source_tree_unique_ptr = std::unique_ptr<disk_source_tree_type>;
        using compiler_importer_type = google::protobuf::compiler::Importer;

        [[nodiscard]] static std::string get_disk_path(const std::string& proto_file, const path_type& parent_path);

        [[nodiscard]] static disk_source_tree_unique_ptr get_disk_source_tree(const path_type& parent_path);

        std::string proto_file_;
        std::string proto_directory_;
        path_type current_path_;
        path_type parent_path_;
        std::string disk_path_;
        importer_error_collector error_collector_;
        disk_source_tree_unique_ptr disk_source_tree_;
        compiler_importer_type compiler_importer_;
    };
}