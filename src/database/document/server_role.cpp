#include "server_role.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::server_role::server_role()
    : game_server_id_{}, role_name_{}, last_login_time_{}
{
}

std::string celeritas::server_role::get_game_server_id() const
{
    return game_server_id_;
}

void celeritas::server_role::set_game_server_id(const std::string& gameServerId)
{
    game_server_id_ = gameServerId;
}

std::string celeritas::server_role::get_role_name() const
{
    return role_name_;
}

void celeritas::server_role::set_role_name(const std::string& roleName)
{
    role_name_ = roleName;
}

int64_t celeritas::server_role::get_last_login_time() const
{
    return last_login_time_;
}

void celeritas::server_role::set_last_login_time(const int64_t lastLoginTime)
{
    last_login_time_ = lastLoginTime;
}

std::string celeritas::server_role::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(game_server_id_description, game_server_id_));
    builder.append(bsoncxx::builder::basic::kvp(role_name_description, role_name_));
    builder.append(bsoncxx::builder::basic::kvp(last_login_time_description, last_login_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::server_role celeritas::server_role::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    server_role role{};
    role.set_game_server_id(std::string{ parsed_view[game_server_id_description].get_string().value });
    role.set_role_name(std::string{ parsed_view[role_name_description].get_string().value });
    role.set_last_login_time(parsed_view[last_login_time_description].get_int64());

    return role;
}