#pragma once

#include "handler_template_file.h"

#include <map>
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
                              std::string template_directory,
                              const handler_template_file& handler_template_file);

        void generate_file();

    private:
        using field_type = std::map<std::string, std::string>;

        void generate_file(std::string_view file_name, std::string_view message_full_name, std::string_view message_name, std::string_view one_of_name, const field_type& field);

        void generate_header_file(std::string_view file_name, std::string_view message_full_name, std::string_view message_name);

        void generate_source_file(const std::string_view message_full_name, std::string_view message_name, std::string_view one_of_name, const field_type& field);

        std::string proto_file_;
        std::string proto_directory_;
        std::string output_directory_;
        std::string template_directory_;
        const handler_template_file& handler_template_file_;
    };
}