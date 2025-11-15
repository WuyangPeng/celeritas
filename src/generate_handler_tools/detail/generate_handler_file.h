#pragma once

#include "handler_template_file.h"

#include <google/protobuf/descriptor.h>

#include <set>
#include <string>

namespace celeritas
{
    class generate_handler_file
    {
    public:
        using class_type = generate_handler_file;

        generate_handler_file(std::string proto_file,
                              std::string proto_directory,
                              std::string output_directory,
                              const handler_template_file& handler_template_file);

        void generate_file();

    private:
        using field_type = std::set<std::string>;
        using file_descriptor_type = const google::protobuf::FileDescriptor*;
        using descriptor_type = const google::protobuf::Descriptor*;
        using one_of_descriptor_type = const google::protobuf::OneofDescriptor*;

        void generate_file(file_descriptor_type file_descriptor) const;

        void generate_file(file_descriptor_type file_descriptor, descriptor_type message_descriptor) const;

        void generate_file(one_of_descriptor_type one_of_descriptor, file_descriptor_type file_descriptor, descriptor_type message_descriptor) const;

        [[nodiscard]] static field_type get_field(one_of_descriptor_type one_of_descriptor);

        void generate_file(std::string_view file_name, std::string_view message_full_name, std::string_view message_name, std::string_view one_of_name, const field_type& field) const;

        void generate_header_file(std::string_view file_name, std::string_view message_full_name, std::string_view message_name) const;

        void generate_source_file(std::string_view message_full_name, std::string_view message_name, std::string_view one_of_name, const field_type& field) const;

        std::string proto_file_;
        std::string proto_directory_;
        std::string output_directory_;
        const handler_template_file& handler_template_file_;
    };
}