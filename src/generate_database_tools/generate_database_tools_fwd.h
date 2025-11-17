#pragma once

#include <string>

namespace celeritas
{
    class generate_database_tools;
    class process;
    class generate_database;

    constexpr std::string_view process_command_line{ "process" };
    constexpr std::string_view process_description{ "database procedure for tool usage." };
    constexpr std::string_view process_generate_database{ "generate_database" };
    constexpr std::string_view database_directory_command_line{ "database_directory" };
    constexpr std::string_view database_directory_description{ "database directory path for program execution." };
    constexpr std::string_view output_directory_command_line{ "output_directory" };
    constexpr std::string_view output_directory_description{ "output directory path for program execution." };
    constexpr std::string_view template_directory_command_line{ "template_directory" };
    constexpr std::string_view template_directory_description{ "template directory path for program execution." };

    constexpr std::string_view entity_h_template_name{ "entity_h.txt" };
    constexpr std::string_view entity_cpp_template_name{ "entity_cpp.txt" };
    constexpr std::string_view database_describe_template_name{ "database_describe.txt" };
    constexpr std::string_view database_get_declaration_template_name{ "database_get_declaration.txt" };
    constexpr std::string_view database_set_declaration_template_name{ "database_set_declaration.txt" };
    constexpr std::string_view database_modify_declaration_template_name{ "database_modify_declaration.txt" };
    constexpr std::string_view database_get_define_template_name{ "database_get_define.txt" };
    constexpr std::string_view database_set_define_template_name{ "database_set_define.txt" };
    constexpr std::string_view database_modify_define_template_name{ "database_modify_define.txt" };
    constexpr std::string_view field_template_name{ "field.txt" };
    constexpr std::string_view field_assignment_template_name{ "field_assignment.txt" };
    constexpr std::string_view field_init_template_name{ "field_init.txt" };
    constexpr std::string_view database_field_template_name{ "database_field.txt" };
}