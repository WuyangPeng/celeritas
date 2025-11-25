#pragma once

#include <format>
#include <stdexcept>
#include <string>

namespace celeritas
{
    class celeritas_error : public std::runtime_error
    {
    public:
        using class_type = celeritas_error;
        using base_type = std::runtime_error;

        template <typename... Args>
        explicit celeritas_error(std::format_string<Args...> fmt, Args&&... args)
            : base_type{ std::vformat(fmt.get(), std::make_format_args(args...)) }
        {
        }

        explicit celeritas_error(const std::string& error);
    };
}
