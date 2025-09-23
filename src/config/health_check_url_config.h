#pragma once

#include <string>

namespace celeritas
{
    class health_check_url_config
    {
    public:
        using class_type = health_check_url_config;

    private:
        std::string url_;
        int interval_ = 0;
        int timeout_ = 0;
    };
}



