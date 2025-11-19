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
        void generate_file(const database_attribute& attribute);

        void generate_entity_h_file(const database_attribute& attribute);

        void generate_entity_cpp_file(const database_attribute& attribute);

        std::string relative_path_;
        std::string database_file_;
        std::string output_directory_;
        const database_template_file& database_template_file_;
    };
}