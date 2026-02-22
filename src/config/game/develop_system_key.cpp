#include "develop_system_key.h"

celeritas::develop_system_key::develop_system_key(const config::develop_system_type develop_system, const config::develop_sub_type develop_sub)
    : develop_system_{ develop_system }, develop_sub_{ develop_sub }
{
}

celeritas::config::develop_system_type celeritas::develop_system_key::get_develop_system() const
{
    return develop_system_;
}

celeritas::config::develop_sub_type celeritas::develop_system_key::get_develop_sub() const
{
    return develop_sub_;
}

bool celeritas::operator<(const develop_system_key& lhs, const develop_system_key& rhs)
{
    if (lhs.get_develop_system() < rhs.get_develop_system())
    {
        return true;
    }

    if (rhs.get_develop_system() < lhs.get_develop_system())
    {
        return false;
    }

    return lhs.get_develop_sub() < rhs.get_develop_sub();
}