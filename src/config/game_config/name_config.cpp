#include "name_config.h"

celeritas::name_config::name_config(const int id, std::string name, const sex_type sex_type, const int weight)
    : id_{ id }, name_{ std::move(name) }, sex_type_{ sex_type }, weight_{ weight }
{
}

int celeritas::name_config::get_id() const
{
    return id_;
}

std::string celeritas::name_config::get_name() const
{
    return name_;
}

celeritas::config::sex_type celeritas::name_config::get_sex_type() const
{
    return sex_type_;
}

int celeritas::name_config::get_weight() const
{
    return weight_;
}