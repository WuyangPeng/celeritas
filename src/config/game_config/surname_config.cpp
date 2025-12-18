#include "surname_config.h"

celeritas::surname_config::surname_config(const int id, std::string name, const int weight)
    : id_{ id }, name_{ std::move(name) }, weight_{ weight }
{
}

int celeritas::surname_config::get_id() const
{
    return id_;
}

std::string celeritas::surname_config::get_name() const
{
    return name_;
}

int celeritas::surname_config::get_weight() const
{
    return weight_;
}