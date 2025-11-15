#pragma once

#include "process.h"

namespace celeritas
{
    class generate_register_handler_helper final : public process
    {
    public:
        using class_type = generate_register_handler_helper;
        using base_type = process;

        explicit generate_register_handler_helper(const command_line_config& command_line_config);

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
        handler_file http_handler_file_;
    };
}