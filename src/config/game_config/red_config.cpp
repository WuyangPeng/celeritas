#include "red_config.h"

celeritas::red_config::red_config(const red_dot_type id, std::string name, const red_dot_type parent_node_id, const red_dot_status_type red_dot_status_type, bool save_database)
    : id_{ id }, name_{ std::move(name) }, parent_node_id_{ parent_node_id }, red_dot_status_type_{ red_dot_status_type }, save_database_{ save_database }
{
}

celeritas::red_dot_type celeritas::red_config::get_id() const
{
    return id_;
}

std::string celeritas::red_config::get_name() const
{
    return name_;
}

celeritas::red_dot_type celeritas::red_config::get_parent_node_id() const
{
    return parent_node_id_;
}

celeritas::red_dot_status_type celeritas::red_config::get_red_dot_status_type() const
{
    return red_dot_status_type_;
}

bool celeritas::red_config::is_save_database() const
{
    return save_database_;
}