#pragma once

#include "generator.h"

#include <vector>

namespace celeritas
{
    class generate_handler_header_file final : public generator
    {
    public:
        using class_type = generate_handler_header_file;
        using base_type = generator;
        using handler_file = std::vector<std::string>;

        generate_handler_header_file(std::string handler_directory, std::string output_directory, std::string template_directory, handler_file handler_file);

        void execute() override;

    private:
        std::string handler_directory_;
        std::string output_directory_;
        std::string template_directory_;
        handler_file handler_file_;
    };
}