#pragma once

#include <string>

namespace celeritas
{
    class health_check_url_config
    {
    public:
        using class_type = health_check_url_config;

        health_check_url_config() noexcept = default;

        health_check_url_config(std::string url, int interval, int timeout);

    private:
        std::string url_;
        int interval_ = 0;
        int timeout_ = 0;
    };
}



