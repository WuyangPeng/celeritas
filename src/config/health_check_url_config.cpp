#include "health_check_url_config.h"

celeritas::health_check_url_config::health_check_url_config(std::string url, const int interval, const int timeout)
    : url_{ std::move(url) }, interval_{ interval }, timeout_{ timeout }
{
}