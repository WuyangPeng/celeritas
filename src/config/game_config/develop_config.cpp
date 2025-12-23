#include "develop_config.h"

celeritas::develop_config::develop_config(const int id, const develop_system_type develop_system_type, const develop_sub_type develop_sub_type, const int max_level, const develop_reset_type develop_reset_type)
    : id_{ id }, develop_system_type_{ develop_system_type }, develop_sub_type_{ develop_sub_type }, max_level_{ max_level }, develop_reset_type_{ develop_reset_type }
{
}

int celeritas::develop_config::get_id() const
{
    return id_;
}

celeritas::develop_system_type celeritas::develop_config::get_develop_system_type() const
{
    return develop_system_type_;
}

celeritas::develop_sub_type celeritas::develop_config::get_develop_sub_type() const
{
    return develop_sub_type_;
}

int celeritas::develop_config::get_max_level() const
{
    return max_level_;
}

celeritas::develop_reset_type celeritas::develop_config::get_develop_reset_type() const
{
    return develop_reset_type_;
}