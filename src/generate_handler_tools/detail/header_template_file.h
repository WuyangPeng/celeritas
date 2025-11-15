#pragma once

#include <string>

namespace celeritas
{
    class header_template_file
    {
    public:
        using class_type = header_template_file;

        explicit header_template_file(std::string template_directory);

        [[nodiscard]] std::string get_handler_generated_h_content() const;

    private:
        void load_template();

        std::string template_directory_;
        std::string handler_generated_h_content_;
    };
}