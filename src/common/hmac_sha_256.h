#pragma once

#include <sstream>
#include <string>

namespace celeritas
{
    class hmac_sha256
    {
    public:
        using class_type = hmac_sha256;

        [[nodiscard]] static std::string calculate(const std::string& secret_key, const std::string& data);

        template <typename... Args>
        [[nodiscard]] static std::string calculate_with_args(const std::string& secret_key, Args&&... args)
        {
            std::ostringstream os{};

            ((os << std::forward<Args>(args)), ...);

            return calculate(secret_key, os.str());
        }
    };
}