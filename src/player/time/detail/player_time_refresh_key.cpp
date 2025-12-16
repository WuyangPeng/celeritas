#include "player_time_refresh_key.h"

celeritas::player_time_refresh_key::player_time_refresh_key(const time_refresh_type time_refresh_type, const int64_t parameter, const int64_t time_id)
    : time_refresh_type_{ time_refresh_type }, parameter_{ parameter }, time_id_{ time_id }
{
}

celeritas::player_time_refresh_key::player_time_refresh_key(const player_time_refresh& player_time_refresh)
    : time_refresh_type_{ player_time_refresh.get_time_refresh_type() }, parameter_{ player_time_refresh.get_parameter() }, time_id_{ player_time_refresh.get_time_id() }
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

int64_t celeritas::player_time_refresh_key::get_time_id() const
{
    return time_id_;
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

    if (lhs.get_parameter() < rhs.get_parameter())
    {
        return true;
    }

    if (lhs.get_parameter() > rhs.get_parameter())
    {
        return false;
    }

    return lhs.get_time_id() < rhs.get_time_id();
}