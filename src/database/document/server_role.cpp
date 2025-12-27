#include "server_role.h"
#include "common/time_helper.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <utility>

celeritas::server_role::server_role()
    : game_server_id_{}, role_surname_{}, role_name_{}, last_login_time_{}
{
}

celeritas::server_role::server_role(std::string game_server_id, std::string role_surname, std::string role_name)
    : game_server_id_{ std::move(game_server_id) }, role_surname_{ std::move(role_surname) }, role_name_{ std::move(role_name) }, last_login_time_{ time_helper::get_current_milliseconds() }
{
}

std::string celeritas::server_role::get_game_server_id() const
{
    return game_server_id_;
}

void celeritas::server_role::set_game_server_id(const std::string& game_server_id)
{
    game_server_id_ = game_server_id;
}

std::string celeritas::server_role::get_role_surname() const
{
    return role_surname_;
}

void celeritas::server_role::set_role_surname(const std::string& role_surname)
{
    role_surname_ = role_surname;
}

std::string celeritas::server_role::get_role_name() const
{
    return role_name_;
}

void celeritas::server_role::set_role_name(const std::string& role_name)
{
    role_name_ = role_name;
}

int64_t celeritas::server_role::get_last_login_time() const
{
    return last_login_time_;
}

void celeritas::server_role::set_last_login_time(const int64_t last_login_time)
{
    last_login_time_ = last_login_time;
}

std::string celeritas::server_role::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ game_server_id_description }, game_server_id_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ role_surname_description }, role_surname_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ role_name_description }, role_name_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ last_login_time_description }, last_login_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::server_role celeritas::server_role::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    server_role role{};
    role.set_game_server_id(std::string{ parsed_view[game_server_id_description].get_string().value });
    role.set_role_surname(std::string{ parsed_view[role_surname_description].get_string().value });
    role.set_role_name(std::string{ parsed_view[role_name_description].get_string().value });

    if (const auto& last_login_time_element = parsed_view[last_login_time_description];
        last_login_time_element.type() == bsoncxx::type::k_int32)
    {
        role.set_last_login_time(last_login_time_element.get_int32());
    }
    else
    {
        role.set_last_login_time(last_login_time_element.get_int64());
    }

    return role;
}