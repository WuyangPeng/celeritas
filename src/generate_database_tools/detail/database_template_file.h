#pragma once

#include <string>

namespace celeritas
{
    class database_template_file
    {
    public:
        using class_type = database_template_file;

        explicit database_template_file(std::string template_directory);

        [[nodiscard]] std::string get_entity_h_content() const;

        [[nodiscard]] std::string get_entity_cpp_content() const;

        [[nodiscard]] std::string get_database_describe_content() const;

        [[nodiscard]] std::string get_database_get_declaration_content() const;

        [[nodiscard]] std::string get_database_set_declaration_content() const;

        [[nodiscard]] std::string get_database_modify_declaration_content() const;

        [[nodiscard]] std::string get_database_array_modify_declaration_content() const;

        [[nodiscard]] std::string get_database_get_define_content() const;

        [[nodiscard]] std::string get_database_set_define_content() const;

        [[nodiscard]] std::string get_database_modify_define_content() const;

        [[nodiscard]] std::string get_database_array_modify_define_content() const;

        [[nodiscard]] std::string get_field_content() const;

        [[nodiscard]] std::string get_field_assignment_content() const;

        [[nodiscard]] std::string get_field_init_content() const;

        [[nodiscard]] std::string get_database_field_content() const;

        [[nodiscard]] std::string get_mongo_database_field_content() const;

        [[nodiscard]] std::string get_database_add_modify_content() const;

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
        std::string database_array_modify_declaration_content_;
        std::string database_get_define_content_;
        std::string database_set_define_content_;
        std::string database_modify_define_content_;
        std::string database_array_modify_define_content_;
        std::string field_content_;
        std::string field_assignment_content_;
        std::string field_init_content_;
        std::string database_field_content_;
        std::string mongo_database_field_content_;
        std::string database_add_modify_content_;
    };
}