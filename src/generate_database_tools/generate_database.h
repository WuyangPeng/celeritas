#pragma once

#include "process.h"
#include "detail/database_template_file.h"

namespace celeritas
{
    class generate_database final : public process
    {
    public:
        using class_type = generate_database;
        using base_type = process;

        explicit generate_database(const command_line_config& command_line_config);

        void execute() override;

    private:
        void check_directory() const;

        void generate(const std::string& directory);

        void generate_file(const std::string& relative_path, const std::string& database_file) const;

        std::string database_directory_;
        std::string output_directory_;
        std::string template_directory_;
        database_template_file database_template_file_;
    };
}