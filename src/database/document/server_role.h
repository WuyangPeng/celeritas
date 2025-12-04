#pragma once

#include <cstdint>
#include <string>

namespace celeritas
{
    class server_role
    {
    public:
        using class_type = server_role;

        server_role();

        [[nodiscard]] std::string get_game_server_id() const;

        void set_game_server_id(const std::string& gameServerId);

        [[nodiscard]] std::string get_role_name() const;

        void set_role_name(const std::string& roleName);

        [[nodiscard]] int64_t get_last_login_time() const;

        void set_last_login_time(int64_t lastLoginTime);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static server_role from_json_string(const std::string& json_string);

        static constexpr std::string_view game_server_id_description = "game_server_id";
        static constexpr std::string_view role_name_description = "role_name";
        static constexpr std::string_view last_login_time_description = "last_login_time";

    private:
        std::string game_server_id_;
        std::string role_name_;
        int64_t last_login_time_;
    };
}