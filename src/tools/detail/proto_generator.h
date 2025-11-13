#pragma once

#include "handler_template_file.h"

#include <string>

namespace celeritas
{
    class proto_generator
    {
    public:
        using class_type = proto_generator;

        proto_generator(std::string_view message_full_name,
                        std::string_view message_name,
                        std::string output_directory,
                        const handler_template_file& handler_template_file);

        virtual ~proto_generator() noexcept = default;

        proto_generator(const proto_generator& rhs) noexcept = default;

        proto_generator& operator=(const proto_generator& rhs) noexcept = delete;

        proto_generator(proto_generator&& rhs) noexcept = default;

        proto_generator& operator=(proto_generator&& rhs) noexcept = delete;

        virtual void execute() = 0;

        [[nodiscard]] std::string get_message_name() const;

        [[nodiscard]] std::string get_message_full_name() const;

        [[nodiscard]] std::string get_output_directory() const;

        [[nodiscard]] std::string get_message_handler_h_content() const;

        [[nodiscard]] std::string get_message_handler_cpp_content() const;

        [[nodiscard]] std::string get_add_handler_function_content() const;

    private:
        std::string message_full_name_;
        std::string message_name_;
        std::string output_directory_;
        const handler_template_file& handler_template_file_;
    };
}