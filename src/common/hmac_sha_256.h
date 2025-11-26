#pragma once

#include <string>

namespace celeritas
{
    class hmac_sha256
    {
    public:
        using class_type = hmac_sha256;

        [[nodiscard]] static std::string calculate(const std::string& data, const std::string& secret_key);
    };
}