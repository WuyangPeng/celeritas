#include "redis_parameter.h"

celeritas::redis_parameter::redis_parameter(const std::string_view host, const int port, const std::string_view user, const std::string_view password, const std::string_view db_name, const int expire_seconds)
    : host_{ host }, port_{ port }, user_{ user }, password_{ password }, db_name_{ db_name }, expire_seconds_{ expire_seconds }
{
}

std::string celeritas::redis_parameter::get_host() const
{
    return host_;
}

int celeritas::redis_parameter::get_port() const
{
    return port_;
}

std::string celeritas::redis_parameter::get_user() const
{
    return user_;
}

std::string celeritas::redis_parameter::get_password() const
{
    return password_;
}

std::string celeritas::redis_parameter::get_db_name() const
{
    return db_name_;
}

int celeritas::redis_parameter::get_expire_seconds() const
{
    return expire_seconds_;
}

std::string celeritas::redis_parameter::get_auth_command() const
{
    return user_.empty() ? "AUTH " + password_ : "AUTH " + user_ + " " + password_;
}

std::string celeritas::redis_parameter::get_prefixed_key(const std::string& key) const
{
    if (db_name_.empty())
    {
        return key;
    }

    return db_name_ + ":" + key;
}

std::string celeritas::redis_parameter::get_expire_seconds_command(int expire_seconds) const
{
    if (expire_seconds == 0)
    {
        expire_seconds = expire_seconds_;
    }

    if (expire_seconds > 0)
    {
        return " EX " + std::to_string(expire_seconds);
    }

    return "";
}