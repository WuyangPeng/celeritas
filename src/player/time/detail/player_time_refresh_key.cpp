#include "player_time_refresh_key.h"

celeritas::player_time_refresh_key::player_time_refresh_key(const time_refresh_type time_refresh_type, const int64_t parameter1, const int64_t parameter2)
    : time_refresh_type_{ time_refresh_type }, parameter1_{ parameter1 }, parameter2_{ parameter2 }
{
}

celeritas::player_time_refresh_key::player_time_refresh_key(const player_time_refresh& player_time_refresh)
    : time_refresh_type_{ player_time_refresh.get_time_refresh_type() }, parameter1_{ player_time_refresh.get_parameter1() }, parameter2_{ player_time_refresh.get_parameter2() }
{
}

celeritas::time_refresh_type celeritas::player_time_refresh_key::get_time_refresh_type() const
{
    return time_refresh_type_;
}

int64_t celeritas::player_time_refresh_key::get_parameter1() const
{
    return parameter1_;
}

int64_t celeritas::player_time_refresh_key::get_parameter2() const
{
    return parameter2_;
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

    if (lhs.get_parameter1() < rhs.get_parameter1())
    {
        return true;
    }

    if (lhs.get_parameter1() > rhs.get_parameter1())
    {
        return false;
    }

    return lhs.get_parameter2() < rhs.get_parameter2();
}