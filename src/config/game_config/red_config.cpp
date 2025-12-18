#include "red_config.h"

celeritas::red_config::red_config(const red_type id, std::string name, const red_type parent_node_id, const red_status_type red_status_type)
    : id_{ id }, name_{ std::move(name) }, parent_node_id_{ parent_node_id }, red_status_type_{ red_status_type }
{
}

celeritas::red_type celeritas::red_config::get_id() const
{
    return id_;
}

std::string celeritas::red_config::get_name() const
{
    return name_;
}

celeritas::red_type celeritas::red_config::get_parent_node_id() const
{
    return parent_node_id_;
}

celeritas::red_status_type celeritas::red_config::get_red_status_type() const
{
    return red_status_type_;
}