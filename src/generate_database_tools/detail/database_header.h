#pragma once

#include "generate_database_tools_internal_fwd.h"

#include <string>

namespace celeritas
{
    class database_header
    {
    public:
        using class_type = database_header;

        void generate(const database_attribute& attribute, const database_template_file& database_template_file);

        [[nodiscard]] const std::string& get_database_get_declaration() const noexcept;

        [[nodiscard]] const std::string& get_database_set_declaration() const noexcept;

        [[nodiscard]] const std::string& get_database_modify_declaration() const noexcept;

        [[nodiscard]] const std::string& get_database_array_modify_declaration() const;

        [[nodiscard]] const std::string& get_database_describe() const noexcept;

        [[nodiscard]] const std::string& get_field() const noexcept;

    private:
        [[nodiscard]] static std::string create_database_get_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_set_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_modify_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_array_modify_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_describe_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_field_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        std::string database_get_declaration_;
        std::string database_set_declaration_;
        std::string database_modify_declaration_;
        std::string database_array_modify_declaration_;
        std::string database_describe_;
        std::string field_;
    };
}
