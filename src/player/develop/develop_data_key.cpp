#include "develop_data_key.h"

celeritas::develop_data_key::develop_data_key(const int system_id, const int64_t instance_id)
    : system_id_{ system_id }, instance_id_{ instance_id }
{
}

int celeritas::develop_data_key::get_system_id() const
{
    return system_id_;
}

int64_t celeritas::develop_data_key::get_instance_id() const
{
    return instance_id_;
}

bool celeritas::operator<(const develop_data_key& lhs, const develop_data_key& rhs)
{
    if (lhs.get_system_id() < rhs.get_system_id())
    {
        return true;
    }

    if (rhs.get_system_id() < lhs.get_system_id())
    {
        return false;
    }

    return lhs.get_instance_id() < rhs.get_instance_id();
}