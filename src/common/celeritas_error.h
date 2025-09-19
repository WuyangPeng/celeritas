#pragma once

#include <stdexcept>

namespace celeritas
{
    class celeritas_error final : public std::runtime_error
    {
    public:
        using class_type = celeritas_error;
        using base_type = std::runtime_error;

        explicit celeritas_error(const std::string& error);
    };
}
