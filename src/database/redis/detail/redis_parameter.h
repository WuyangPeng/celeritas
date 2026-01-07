#pragma once

#include <string>
#include <vector>

namespace celeritas
{
    class redis_parameter
    {
    public:
        using class_type = redis_parameter;
        using array_type = std::vector<std::string>;

        redis_parameter(std::string_view host,
                        int port,
                        std::string_view user,
                        std::string_view password,
                        std::string_view db_name,
                        int expire_seconds);

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_user() const;

        [[nodiscard]] std::string get_password() const;

        [[nodiscard]] std::string get_db_name() const;

        [[nodiscard]] int get_expire_seconds() const;

        [[nodiscard]] array_type get_auth_command() const;

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] array_type get_expire_seconds_command(int expire_seconds) const;

    private:
        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int expire_seconds_ = 0;
    };
}