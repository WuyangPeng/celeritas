#pragma once

#include "process.h"

namespace celeritas
{
    class generate_handler_header final : public process
    {
    public:
        using class_type = generate_handler_header;
        using base_type = process;

        explicit generate_handler_header(const command_line_config& command_line_config);

        void execute() override;

    private:
        using handler_file = std::vector<std::string>;

        void check_directory();

        void set_handler_file(const std::string& directory);

        void generate();

        std::string handler_directory_;
        std::string output_directory_;
        std::string template_directory_;
        handler_file handler_file_;
    };
}