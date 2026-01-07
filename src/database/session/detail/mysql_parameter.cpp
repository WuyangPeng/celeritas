#include "mysql_parameter.h"

celeritas::mysql_parameter::mysql_parameter(const std::string_view host, const int port, const std::string_view user, const std::string_view password, const std::string_view db_name)
    : host_{ host }, port_{ port }, user_{ user }, password_{ password }, db_name_{ db_name }
{
}

std::string celeritas::mysql_parameter::get_host() const
{
    return host_;
}

int celeritas::mysql_parameter::get_port() const
{
    return port_;
}

std::string celeritas::mysql_parameter::get_user() const
{
    return user_;
}

std::string celeritas::mysql_parameter::get_password() const
{
    return password_;
}

std::string celeritas::mysql_parameter::get_db_name() const
{
    return db_name_;
}

celeritas::mysql_parameter::connect_params_type celeritas::mysql_parameter::get_connect_params_type() const
{
    connect_params_type connect_params{};
    connect_params.server_address.emplace_host_and_port(host_, port_);
    connect_params.username = user_;
    connect_params.password = password_;
    connect_params.database = db_name_;

    return connect_params;
}