#pragma once

#include "generate_database_tools_internal_fwd.h"
#include "database_template_file.h"
#include <string>

namespace celeritas
{
    class database_header
    {
    public:
        using class_type = database_header;

        void generate(const database_attribute& attribute, const database_template_file& database_template_file);

        [[nodiscard]] const std::string& get_database_get_declaration() const noexcept { return database_get_declaration_; }
        [[nodiscard]] const std::string& get_database_set_declaration() const noexcept { return database_set_declaration_; }
        [[nodiscard]] const std::string& get_database_modify_declaration() const noexcept { return database_modify_declaration_; }
        [[nodiscard]] const std::string& get_database_describe() const noexcept { return database_describe_; }
        [[nodiscard]] const std::string& get_field() const noexcept { return field_; }

    private:
        std::string database_get_declaration_;
        std::string database_set_declaration_;
        std::string database_modify_declaration_;
        std::string database_describe_;
        std::string field_;
    };
}
