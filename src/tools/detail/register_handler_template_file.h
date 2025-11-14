#pragma once

#include <string>

namespace celeritas
{
    class register_handler_template_file
    {
    public:
        using class_type = register_handler_template_file;

        explicit register_handler_template_file(std::string template_directory);

        [[nodiscard]] std::string get_register_handler_helper_cpp_content() const;

    private:
        void load_template();

        std::string template_directory_;
        std::string register_handler_helper_cpp_content_;
    };
}