#pragma once

#include <boost/mysql/connect_params.hpp>

#include <string>

namespace celeritas
{
    class mysql_parameter
    {
    public:
        using class_type = mysql_parameter;
        using connect_params_type = boost::mysql::connect_params;

        mysql_parameter(std::string_view host,
                        int port,
                        std::string_view user,
                        std::string_view password,
                        std::string_view db_name);

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_user() const;

        [[nodiscard]] std::string get_password() const;

        [[nodiscard]] std::string get_db_name() const;

        [[nodiscard]] connect_params_type get_connect_params_type() const;

    private:
        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
    };
}