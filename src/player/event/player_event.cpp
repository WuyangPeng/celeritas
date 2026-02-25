#include "player_event.h"
#include "common/core/time_helper.h"

celeritas::player_event::player_event(const player_event_type type, const bool is_login)
    : type_{ type }, timestamp_{ time_helper::get_current_milliseconds() }, is_login_{ is_login }, data_{}
{
}

celeritas::player_event_type celeritas::player_event::get_type() const noexcept
{
    return type_;
}

int64_t celeritas::player_event::get_timestamp() const noexcept
{
    return timestamp_;
}

bool celeritas::player_event::is_login() const noexcept
{
    return is_login_;
}

void celeritas::player_event::set_is_login(const bool is_login) noexcept
{
    is_login_ = is_login;
}

bool celeritas::player_event::has_data(const std::string& key) const
{
    return data_.contains(key);
}
