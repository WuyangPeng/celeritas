#include "log_param.h"

celeritas::log_param::log_param(int64_t player_id, int64_t timestamp, std::string ip_address)
    : player_id_{ player_id }, timestamp_{ timestamp }, ip_address_{ std::move(ip_address) }
{
}

int64_t celeritas::log_param::get_player_id() const noexcept
{
    return player_id_;
}

int64_t celeritas::log_param::get_timestamp() const noexcept
{
    return timestamp_;
}

const std::string& celeritas::log_param::get_ip_address() const noexcept
{
    return ip_address_;
}
