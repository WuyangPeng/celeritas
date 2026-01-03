#pragma once

#include "config/config_constant.h"

#include <string>

namespace celeritas
{
    class health_check_url_config
    {
    public:
        using class_type = health_check_url_config;

        health_check_url_config() noexcept = default;

        health_check_url_config(std::string url, int interval, int timeout) noexcept;

        [[nodiscard]] std::string get_url() const;

        [[nodiscard]] int get_interval() const noexcept;

    private:
        std::string url_;
        int interval_ = default_health_check_interval;
        int timeout_ = default_health_check_timeout;
    };
}



