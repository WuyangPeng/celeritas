#include "login_server_info.h"
#include "common/enum_cast.h"

celeritas::login_server_info::login_server_info(std::string game_server_id,
                                                std::string server_name,
                                                const server_status_type server_status,
                                                connection_info connection_info,
                                                player_role optional_player)
    : game_server_id_{ std::move(game_server_id) },
      server_name_{ std::move(server_name) },
      server_status_{ server_status },
      connection_info_{ std::move(connection_info) },
      player_role_{ std::move(optional_player) }
{
}

celeritas::login_server_info::login_server_info(std::string game_server_id,
                                                std::string server_name,
                                                server_status_type server_status,
                                                connection_info connection_info)
    : game_server_id_{ std::move(game_server_id) },
      server_name_{ std::move(server_name) },
      server_status_{ server_status },
      connection_info_{ std::move(connection_info) },
      player_role_{}
{
}

celeritas::login_server_info::login_server_info(std::string game_server_id,
                                                std::string server_name,
                                                const server_status_type server_status)
    : game_server_id_{ std::move(game_server_id) },
      server_name_{ std::move(server_name) },
      server_status_{ server_status },
      connection_info_{},
      player_role_{}
{
}

std::string celeritas::login_server_info::get_game_server_id() const
{
    return game_server_id_;
}

void celeritas::login_server_info::set_game_server_id(const std::string& game_server_id)
{
    game_server_id_ = game_server_id;
}

std::string celeritas::login_server_info::get_server_name() const
{
    return server_name_;
}

void celeritas::login_server_info::set_server_name(const std::string& server_name)
{
    server_name_ = server_name;
}

celeritas::server_status_type celeritas::login_server_info::get_server_status() const
{
    return server_status_;
}

void celeritas::login_server_info::set_server_status(const server_status_type server_status)
{
    server_status_ = server_status;
}

celeritas::login_server_info::optional_connection_info celeritas::login_server_info::get_connection_info() const
{
    return connection_info_;
}

void celeritas::login_server_info::set_connection_info(const connection_info& connection_info)
{
    connection_info_ = connection_info;
}

celeritas::login_server_info::optional_player_role celeritas::login_server_info::get_player_role() const
{
    return player_role_;
}

void celeritas::login_server_info::set_player_role(const player_role& player_role)
{
    player_role_ = player_role;
}

celeritas::login_server_info celeritas::tag_invoke(login_server_info_tag, const login_server_info::json_value& value)
{
    const auto& object = value.as_object();

    const auto game_server_id = boost::json::value_to<std::string>(object.at(login_server_info::game_server_id_description));
    const auto server_name = boost::json::value_to<std::string>(object.at(login_server_info::server_name_description));
    const auto server_status = boost::json::value_to<int>(object.at(login_server_info::server_status_description));

    login_server_info login_server_info{ game_server_id, server_name, underlying_cast_enum<server_status_type>(server_status) };

    if (object.contains(login_server_info::connection_info_description))
    {
        login_server_info.set_connection_info(boost::json::value_to<connection_info>(object.at(login_server_info::connection_info_description)));
    }

    if (object.contains(login_server_info::player_role_description))
    {
        login_server_info.set_player_role(boost::json::value_to<player_role>(object.at(login_server_info::player_role_description)));
    }

    return login_server_info;
}

void celeritas::tag_invoke(boost::json::value_from_tag, login_server_info::json_value& value, const login_server_info& info)
{
    auto& object = value.emplace_object();
    object[login_server_info::game_server_id_description] = info.get_game_server_id();
    object[login_server_info::server_name_description] = info.get_server_name();
    object[login_server_info::server_status_description] = enum_cast_underlying(info.get_server_status());

    if (const auto& connection_info = info.get_connection_info();
        connection_info.has_value())
    {
        object[login_server_info::connection_info_description] = boost::json::value_from(*connection_info);
    }

    if (const auto& player_role = info.get_player_role();
        player_role.has_value())
    {
        object[login_server_info::player_role_description] = boost::json::value_from(*player_role);
    }
}
