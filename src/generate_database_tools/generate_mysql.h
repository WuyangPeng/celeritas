#pragma once

#include "process.h"
#include "detail/generator.h"

namespace celeritas
{
    class generate_mysql final : public process
    {
    public:
        using class_type = generate_mysql;
        using base_type = process;

        explicit generate_mysql(const command_line_config& command_line_config);

        void execute() override;

    private:
        void check_directory() const;

        void generate(const std::string& directory);

        std::string database_directory_;
        std::string output_directory_;
    };
}
