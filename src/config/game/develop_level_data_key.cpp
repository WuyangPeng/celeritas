#include "develop_level_data_key.h"

celeritas::develop_level_data_key::develop_level_data_key(const int system_id, const int64_t instance_id, int level)
    : system_id_{ system_id }, instance_id_{ instance_id }, level_{ level }
{
}

int celeritas::develop_level_data_key::get_system_id() const
{
    return system_id_;
}

int64_t celeritas::develop_level_data_key::get_instance_id() const
{
    return instance_id_;
}

int celeritas::develop_level_data_key::get_level() const
{
    return level_;
}

bool celeritas::operator<(const develop_level_data_key& lhs, const develop_level_data_key& rhs)
{
    if (lhs.get_system_id() < rhs.get_system_id())
    {
        return true;
    }

    if (rhs.get_system_id() < lhs.get_system_id())
    {
        return false;
    }

    if (lhs.get_instance_id() < rhs.get_instance_id())
    {
        return true;
    }

    if (rhs.get_instance_id() < lhs.get_instance_id())
    {
        return false;
    }

    return lhs.get_level() < rhs.get_level();
}