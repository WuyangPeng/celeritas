// 此文件是自动生成，请勿手动修改。

#include "server_cell.h"
#include "config/basic/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::server_cell::server_cell(const database_entity_change& entity)
    : base_type{ entity },
      cell_id_{ entity.get_value<database_data_type::int64_type>(cell_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      server_name_{ entity.get_value<database_data_type::string_type>(server_name_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      launch_time_{ entity.get_value<database_data_type::int64_type>(launch_time_describe) },
      zone_{ entity.get_value<database_data_type::string_type>(zone_describe) },
      is_close_display_{ entity.get_value<database_data_type::bool_type>(is_close_display_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
}

celeritas::server_cell::server_cell(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      cell_id_{ entity.get_value<database_data_type::int64_type>(cell_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      server_name_{ entity.get_value<database_data_type::string_type>(server_name_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      launch_time_{ entity.get_value<database_data_type::int64_type>(launch_time_describe) },
      zone_{ entity.get_value<database_data_type::string_type>(zone_describe) },
      is_close_display_{ entity.get_value<database_data_type::bool_type>(is_close_display_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(cell_id_describe, get_cell_id());
        add_modify(game_server_id_describe, get_game_server_id());
        add_modify(server_name_describe, get_server_name());
        add_modify(app_id_describe, get_app_id());
        add_modify(launch_time_describe, get_launch_time());
        add_modify(zone_describe, get_zone());
        add_modify(is_close_display_describe, is_is_close_display());
        add_modify(status_describe, get_status());
    }
}

celeritas::server_cell::server_cell(const database_type database_type, traits::param_type::int64_type cell_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(cell_id) },
      cell_id_{ cell_id },
      game_server_id_{ traits::string_type{} },
      server_name_{ traits::string_type{} },
      app_id_{ traits::int64_type{} },
      launch_time_{ traits::int64_type{} },
      zone_{ traits::string_type{} },
      is_close_display_{ traits::bool_type{} },
      status_{ traits::int32_type{} }
{
    add_modify(cell_id_describe, cell_id);
}

celeritas::traits::int64_type celeritas::server_cell::get_cell_id() const noexcept
{
    return cell_id_.get_value();
}

celeritas::traits::string_type celeritas::server_cell::get_game_server_id() const
{
    return game_server_id_.get_value();
}

celeritas::traits::string_type celeritas::server_cell::get_server_name() const
{
    return server_name_.get_value();
}

celeritas::traits::int64_type celeritas::server_cell::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::int64_type celeritas::server_cell::get_launch_time() const noexcept
{
    return launch_time_.get_value();
}

celeritas::traits::string_type celeritas::server_cell::get_zone() const
{
    return zone_.get_value();
}

celeritas::traits::bool_type celeritas::server_cell::is_is_close_display() const noexcept
{
    return is_close_display_.get_value();
}

celeritas::traits::int32_type celeritas::server_cell::get_status() const noexcept
{
    return status_.get_value();
}

void celeritas::server_cell::set_cell_id(traits::param_type::int64_type cell_id)
{
    if (cell_id != get_cell_id())
    {
        cell_id_.set_value(cell_id);

        add_modify(cell_id_describe, get_cell_id());
    }
}

void celeritas::server_cell::set_game_server_id(traits::param_type::string_type game_server_id)
{
    if (game_server_id != get_game_server_id())
    {
        game_server_id_.set_value(game_server_id);

        add_modify(game_server_id_describe, get_game_server_id());
    }
}

void celeritas::server_cell::set_server_name(traits::param_type::string_type server_name)
{
    if (server_name != get_server_name())
    {
        server_name_.set_value(server_name);

        add_modify(server_name_describe, get_server_name());
    }
}

void celeritas::server_cell::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::server_cell::set_launch_time(traits::param_type::int64_type launch_time)
{
    if (launch_time != get_launch_time())
    {
        launch_time_.set_value(launch_time);

        add_modify(launch_time_describe, get_launch_time());
    }
}

void celeritas::server_cell::set_zone(traits::param_type::string_type zone)
{
    if (zone != get_zone())
    {
        zone_.set_value(zone);

        add_modify(zone_describe, get_zone());
    }
}

void celeritas::server_cell::set_is_close_display(traits::param_type::bool_type is_close_display)
{
    if (is_close_display != is_is_close_display())
    {
        is_close_display_.set_value(is_close_display);

        add_modify(is_close_display_describe, is_is_close_display());
    }
}

void celeritas::server_cell::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

const celeritas::database_entity::database_field_container& celeritas::server_cell::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(cell_id_)::get_database_field(),
                                                                decltype(game_server_id_)::get_database_field(),
                                                                decltype(server_name_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(launch_time_)::get_database_field(),
                                                                decltype(zone_)::get_database_field(),
                                                                decltype(is_close_display_)::get_database_field(),
                                                                decltype(status_)::get_database_field() };

    return field_name_container;
}

celeritas::server_cell::database_entity_change_const_shared_ptr celeritas::server_cell::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::server_cell::database_entity_change_const_shared_ptr celeritas::server_cell::get_select(const database_type database_type, traits::param_type::int64_type cell_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(cell_id));
}

celeritas::server_cell::database_entity_change_const_shared_ptr celeritas::server_cell::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::server_cell::basis_database_container_const_shared_ptr celeritas::server_cell::get_key_basis_database_container(traits::param_type::int64_type cell_id)
{
    return std::make_shared<basis_database_container>(basis_database{ cell_id_describe, cell_id });
}