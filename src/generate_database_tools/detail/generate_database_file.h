#pragma once

#include "database_template_file.h"

#include <string>

namespace celeritas
{
    class generate_database_file
    {
    public:
        using class_type = generate_database_file;

        generate_database_file(std::string relative_path,
                               std::string database_file,
                               std::string output_directory,
                               const database_template_file& database_template_file);

        void generate_file();

    private:
        std::string relative_path_;
        std::string database_file_;
        std::string output_directory_;
        const database_template_file& database_template_file_;
    };
}