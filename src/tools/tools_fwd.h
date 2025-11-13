#pragma once

#include <string_view>

namespace celeritas
{
    class tools;
    class process;
    class generate_handler;

    constexpr std::string_view process_command_line{ "process" };
    constexpr std::string_view process_description{ "Handling procedure for tool usage." };
    constexpr std::string_view proto_directory_command_line{ "proto_directory" };
    constexpr std::string_view proto_directory_description{ "proto directory path for program execution." };
    constexpr std::string_view handler_directory_command_line{ "handler_directory" };
    constexpr std::string_view handler_directory_description{ "handler directory path for program execution." };
    constexpr std::string_view output_directory_command_line{ "output_directory" };
    constexpr std::string_view output_directory_description{ "output directory path for program execution." };
    constexpr std::string_view template_directory_command_line{ "template_directory" };
    constexpr std::string_view template_directory_description{ "template directory path for program execution." };
    constexpr std::string_view process_generate_handler{ "generate_handler" };
    constexpr std::string_view process_generate_handler_header{ "generate_handler_header" };
    constexpr std::string_view proto_extension{ ".proto" };
    constexpr std::string_view header_extension{ ".h" };
    constexpr std::string_view request_suffix{ "request" };
    constexpr std::string_view response_suffix{ "response" };
    constexpr std::string_view message_handler_h_template_name{ "message_handler_h.txt" };
    constexpr std::string_view message_handler_cpp_template_name{ "message_handler_cpp.txt" };
    constexpr std::string_view add_handler_function_template_name{ "add_handler_function.txt" };
}