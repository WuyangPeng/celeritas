#pragma once

#include <string>

namespace celeritas
{
    class global_config
    {
    public:
        using class_type = global_config;

        global_config() noexcept = default;

        global_config(std::string external_host);

        [[nodiscard]] std::string get_external_host() const;

    private:
        std::string external_host_;
    };
}