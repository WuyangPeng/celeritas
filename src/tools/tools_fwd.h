#pragma once

#include <string_view>

namespace celeritas
{
    class tools;
    class process;
    class generate_handler;

    constexpr std::string_view process_command_line{ "process" };
    constexpr std::string_view process_description{ "Handling procedure for tool usage." };
    constexpr std::string_view directory_command_line{ "directory" };
    constexpr std::string_view directory_description{ "Directory path for program execution." };
    constexpr std::string_view process_generate_handler{ "generate_handler" };
}