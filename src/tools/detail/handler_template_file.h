#pragma once

#include <string>

namespace celeritas
{
    class handler_template_file
    {
    public:
        using class_type = handler_template_file;

        explicit handler_template_file(std::string template_directory);

        [[nodiscard]] std::string get_message_handler_h_content() const;

        [[nodiscard]] std::string get_message_handler_cpp_content() const;

        [[nodiscard]] std::string get_add_handler_function_content() const;

    private:
        void load_template();

        void load_template(std::string_view template_name, std::string& content) const;

        std::string template_directory_;
        std::string message_handler_h_content_;
        std::string message_handler_cpp_content_;
        std::string add_handler_function_content_;
    };
}