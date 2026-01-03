#include "health_check_url_config.h"

celeritas::health_check_url_config::health_check_url_config(std::string url, const int interval, const int timeout) noexcept
    : url_{ std::move(url) }, interval_{ interval }, timeout_{ timeout }
{
}

std::string celeritas::health_check_url_config::get_url() const
{
    return url_;
}

int celeritas::health_check_url_config::get_interval() const noexcept
{
    return interval_;
}