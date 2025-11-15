#pragma once

#include "generator.h"

#include <vector>

namespace celeritas
{
    class generate_register_handler_helper_file final : public generator
    {
    public:
        using class_type = generate_register_handler_helper_file;
        using base_type = generator;
        using handler_name = std::vector<std::string>;

        generate_register_handler_helper_file(std::string handler_directory,
                                              std::string output_directory,
                                              std::string template_directory,
                                              handler_name handler_file,
                                              handler_name http_handler_file);

        void execute() override;

    private:
        std::string handler_directory_;
        std::string output_directory_;
        std::string template_directory_;
        handler_name handler_file_;
        handler_name http_handler_file_;
    };
}