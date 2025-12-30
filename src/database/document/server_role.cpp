#include "server_role.h"
#include "common/core_utilities/time_helper.h"
#include "database/basis_database.tpp"

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

celeritas::server_role::document_type celeritas::server_role::to_document_type() const
{
    document_type document{};

    document.emplace_back(game_server_id_description, game_server_id_);
    document.emplace_back(role_surname_description, role_surname_);
    document.emplace_back(role_name_description, role_name_);
    document.emplace_back(last_login_time_description, last_login_time_);

    return document;
}

celeritas::server_role celeritas::server_role::from_document(const document_type& document)
{
    server_role role{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == game_server_id_description)
        {
            role.set_game_server_id(element.get_value<database_data_type::string_type>());
        }
        if (element.get_field_name() == role_surname_description)
        {
            role.set_role_surname(element.get_value<database_data_type::string_type>());
        }
        if (element.get_field_name() == role_name_description)
        {
            role.set_role_name(element.get_value<database_data_type::string_type>());
        }
        if (element.get_field_name() == last_login_time_description)
        {
            role.set_last_login_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return role;
}
