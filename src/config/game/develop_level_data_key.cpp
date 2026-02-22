#include "develop_level_data_key.h"

celeritas::develop_level_data_key::develop_level_data_key(const int develop_id, const int level)
    : develop_id_{ develop_id }, level_{ level }
{
}

int celeritas::develop_level_data_key::get_develop_id() const
{
    return develop_id_;
}

int celeritas::develop_level_data_key::get_level() const
{
    return level_;
}

bool celeritas::operator<(const develop_level_data_key& lhs, const develop_level_data_key& rhs)
{
    if (lhs.get_develop_id() < rhs.get_develop_id())
    {
        return true;
    }

    if (rhs.get_develop_id() < lhs.get_develop_id())
    {
        return false;
    }

    return lhs.get_level() < rhs.get_level();
}