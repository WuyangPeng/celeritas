#pragma once

#include "connection_info.h"
#include "player_role.h"
#include "auth/core/server_status_type.h"

#include <boost/json.hpp>

#include <optional>
#include <string>

namespace celeritas
{
    class login_server_info
    {
    public:
        using class_type = login_server_info;
        using json_value = boost::json::value;
        using optional_connection_info = std::optional<connection_info>;
        using optional_player_role = std::optional<player_role>;

        login_server_info() noexcept = default;

        login_server_info(std::string game_server_id,
                          std::string server_name,
                          server_status_type server_status,
                          connection_info connection_info,
                          player_role optional_player);

        login_server_info(std::string game_server_id,
                          std::string server_name,
                          server_status_type server_status,
                          connection_info connection_info);

        login_server_info(std::string game_server_id,
                          std::string server_name,
                          server_status_type server_status);

        [[nodiscard]] std::string get_game_server_id() const;

        void set_game_server_id(const std::string& game_server_id);

        [[nodiscard]] std::string get_server_name() const;

        void set_server_name(const std::string& server_name);

        [[nodiscard]] server_status_type get_server_status() const;

        void set_server_status(server_status_type server_status);

        [[nodiscard]] optional_connection_info get_connection_info() const;

        void set_connection_info(const connection_info& connection_info);

        [[nodiscard]] optional_player_role get_player_role() const;

        void set_player_role(const player_role& player_role);

        static constexpr std::string_view game_server_id_description = "game_server_id";
        static constexpr std::string_view server_name_description = "server_name";
        static constexpr std::string_view server_status_description = "server_status";
        static constexpr std::string_view connection_info_description = "connection_info";
        static constexpr std::string_view player_role_description = "player_role";

    private:
        std::string game_server_id_;
        std::string server_name_;
        server_status_type server_status_ = server_status_type::normal;
        optional_connection_info connection_info_;
        optional_player_role player_role_;
    };

    using login_server_info_tag = boost::json::value_to_tag<login_server_info>;

    [[nodiscard]] login_server_info tag_invoke(login_server_info_tag, const login_server_info::json_value& value);

    void tag_invoke(boost::json::value_from_tag, login_server_info::json_value& value, const login_server_info& info);
}
