#include "game_tables.h"
#include "container_config.tpp"

celeritas::game_tables::game_tables()
    : surname_config_{ std::make_shared<surname_container_config>() },
      name_config_{ std::make_shared<name_config_config>() }
{
}

celeritas::game_tables::const_surname_container_config_shared_ptr celeritas::game_tables::get_surname_config() const
{
    return surname_config_;
}

void celeritas::game_tables::set_surname_config(const const_surname_container_config_shared_ptr& surname_config)
{
    surname_config_ = surname_config;
}

celeritas::game_tables::const_name_config_shared_ptr celeritas::game_tables::get_name_config() const
{
    return name_config_;
}

void celeritas::game_tables::set_name_config(const const_name_config_shared_ptr& name_config)
{
    name_config_ = name_config;
}