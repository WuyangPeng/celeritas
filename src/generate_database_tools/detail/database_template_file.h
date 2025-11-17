#pragma once
#include <string>

namespace celeritas
{
    class database_template_file
    {
    public:
        using class_type = database_template_file;

        explicit database_template_file(std::string template_directory);

    private:
        void load_template();

        void load_template(std::string_view template_name, std::string& content) const;

        std::string template_directory_;
        std::string entity_h_content_;
        std::string entity_cpp_content_;
        std::string database_describe_content_;
        std::string database_get_declaration_content_;
        std::string database_set_declaration_content_;
        std::string database_modify_declaration_content_;
        std::string database_get_define_content_;
        std::string database_set_define_content_;
        std::string database_modify_define_content_;
        std::string field_content_;
        std::string field_assignment_content_;
        std::string field_init_content_;
        std::string database_field_content_;
    };
}