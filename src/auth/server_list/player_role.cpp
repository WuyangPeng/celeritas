#include "player_role.h"

celeritas::player_role::player_role(std::string role_surname, std::string role_name, const bool modify_name, const int64_t last_login_time)
    : role_surname_{ std::move(role_surname) }, role_name_{ std::move(role_name) }, modify_name_{ modify_name }, last_login_time_{ last_login_time }
{
}

std::string celeritas::player_role::get_role_name() const
{
    return role_name_;
}

void celeritas::player_role::set_role_name(const std::string& role_name)
{
    role_name_ = role_name;
}

int64_t celeritas::player_role::get_last_login_time() const
{
    return last_login_time_;
}

void celeritas::player_role::set_last_login_time(const int64_t last_login_time)
{
    last_login_time_ = last_login_time;
}

std::string celeritas::player_role::get_role_surname() const
{
    return role_surname_;
}

void celeritas::player_role::set_role_surname(const std::string& roleSurname)
{
    role_surname_ = roleSurname;
}

bool celeritas::player_role::is_modify_name() const
{
    return modify_name_;
}

void celeritas::player_role::set_modify_name(const bool modifyName)
{
    modify_name_ = modifyName;
}

celeritas::player_role celeritas::tag_invoke(player_role_tag, const player_role::json_value& value)
{
    const auto& object = value.as_object();

    return player_role{ boost::json::value_to<std::string>(object.at(player_role::role_surname_description)),
                        boost::json::value_to<std::string>(object.at(player_role::role_name_description)),
                        boost::json::value_to<bool>(object.at(player_role::modify_name_description)),
                        boost::json::value_to<int64_t>(object.at(player_role::last_login_time_description)) };
}

void celeritas::tag_invoke(boost::json::value_from_tag, player_role::json_value& value, const player_role& role)
{
    value = {
        { player_role::role_surname_description, role.get_role_surname() },
        { player_role::role_name_description, role.get_role_name() },
        { player_role::modify_name_description, role.is_modify_name() },
        { player_role::last_login_time_description, role.get_last_login_time() } };
}
