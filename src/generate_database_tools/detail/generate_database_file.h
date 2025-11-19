#pragma once

#include "database_template_file.h"
#include "generator.h"
#include "generate_database_tools_internal_fwd.h"

#include <string>

namespace celeritas
{
    class generate_database_file final : public generator
    {
    public:
        using class_type = generate_database_file;

        generate_database_file(std::string relative_path,
                               std::string database_file,
                               std::string output_directory,
                               const database_template_file& database_template_file);

        void execute() override;

    private:
        struct header_strings
        {
            std::string database_get_declaration;
            std::string database_set_declaration;
            std::string database_modify_declaration;
            std::string database_describe;
            std::string field;
        };

        struct source_strings
        {
            std::string database_get_define;
            std::string database_set_define;
            std::string database_modify_define;
            std::string field_assignment;
            std::string field_init;
            std::string database_field;
        };

        void generate_file(const database_attribute& attribute);

        void generate_entity_h_file(const database_attribute& attribute);

        void generate_entity_cpp_file(const database_attribute& attribute);

        [[nodiscard]] std::string generate_header_content(const database_attribute& attribute) const;

        [[nodiscard]] std::string generate_source_content(const database_attribute& attribute) const;

        [[nodiscard]] header_strings generate_header_snippets(const database_attribute& attribute) const;

        [[nodiscard]] source_strings generate_source_snippets(const database_attribute& attribute) const;

        std::string relative_path_;
        std::string database_file_;
        std::string output_directory_;
        const database_template_file& database_template_file_;
    };
}