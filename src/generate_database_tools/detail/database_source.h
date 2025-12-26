#pragma once

#include "generate_database_tools_internal_fwd.h"

#include <string>

namespace celeritas
{
    class database_source
    {
    public:
        using class_type = database_source;

        void generate(const database_attribute& attribute, const database_template_file& database_template_file);

        [[nodiscard]] const std::string& get_database_get_define() const noexcept;

        [[nodiscard]] const std::string& get_database_set_define() const noexcept;

        [[nodiscard]] const std::string& get_database_modify_define() const noexcept;

        [[nodiscard]] const std::string& get_database_array_modify_define() const noexcept;

        [[nodiscard]] const std::string& get_field_assignment() const noexcept;

        [[nodiscard]] const std::string& get_field_init() const noexcept;

        [[nodiscard]] const std::string& get_database_field() const noexcept;

        [[nodiscard]] const std::string& get_mongo_database_field() const noexcept;

        [[nodiscard]] const std::string& get_database_add_modify() const noexcept;

    private:
        [[nodiscard]] static std::string create_database_get_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_set_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_modify_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_array_modify_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_field_assignment_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_field_init_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_field_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_mongo_database_field_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        [[nodiscard]] static std::string create_database_add_modify_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file);

        std::string database_get_define_;
        std::string database_set_define_;
        std::string database_modify_define_;
        std::string database_array_modify_define_;
        std::string field_assignment_;
        std::string field_init_;
        std::string database_field_;
        std::string mongo_database_field_;
        std::string database_add_modify_;
    };
}
