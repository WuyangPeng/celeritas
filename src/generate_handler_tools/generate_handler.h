#pragma once

#include "process.h"
#include "detail/handler_template_file.h"

namespace celeritas
{
    class generate_handler final : public process
    {
    public:
        using class_type = generate_handler;
        using base_type = process;

        explicit generate_handler(const command_line_config& command_line_config);

        void execute() override;

    private:
        void check_directory() const;

        void generate(const std::string& directory);

        void generate_file(const std::string& proto_file) const;

        std::string proto_directory_;
        std::string output_directory_;
        std::string template_directory_;
        handler_template_file handler_template_file_;
    };
}