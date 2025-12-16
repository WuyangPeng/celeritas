#include "player_time_refresh_key.h"

celeritas::player_time_refresh_key::player_time_refresh_key(const time_refresh_type time_refresh_type, const int64_t parameter)
    : time_refresh_type_{ time_refresh_type }, parameter_{ parameter }
{
}

celeritas::time_refresh_type celeritas::player_time_refresh_key::get_time_refresh_type() const
{
    return time_refresh_type_;
}

int64_t celeritas::player_time_refresh_key::get_parameter() const
{
    return parameter_;
}

bool celeritas::operator<(const player_time_refresh_key& lhs, const player_time_refresh_key& rhs)
{
    if (lhs.get_time_refresh_type() < rhs.get_time_refresh_type())
    {
        return true;
    }

    if (lhs.get_time_refresh_type() > rhs.get_time_refresh_type())
    {
        return false;
    }

    return lhs.get_parameter() < rhs.get_parameter();
}