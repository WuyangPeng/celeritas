#pragma once

#include "generate_database_tools_internal_fwd.h"
#include "generator.h"

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
                               const database_template_file& database_template_file,
                               bool mongo);

        void execute() override;

    private:
        void generate_file(const database_attribute& attribute);

        void generate_entity_h_file(const database_attribute& attribute);

        void generate_entity_cpp_file(const database_attribute& attribute);

        [[nodiscard]] std::string generate_header_content(const database_attribute& attribute) const;

        [[nodiscard]] std::string generate_source_content(const database_attribute& attribute) const;

        std::string relative_path_;
        std::string database_file_;
        std::string output_directory_;
        const database_template_file& database_template_file_;
        bool mongo_;
    };
}
