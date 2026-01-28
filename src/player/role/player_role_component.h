#pragma once

#include "database/document/server_role.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/generated/mongo/player/user_role.h"
#include "message/basic/game_error_type.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"
#include "proto/service/player.pb.h"

namespace celeritas
{
    class player_role_component final : public player_component
    {
    public:
        using class_type = player_role_component;
        using base_type = player_component;
        using service_login_request_type = proto::service::service_login_request;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using game_error_awaitable_type = boost::asio::awaitable<game_error_type>;

        explicit player_role_component(player_state* player_state, const service_login_request_type& login) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::role;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] game_error_awaitable_type change_name(const std::string& surname, const std::string& name);

        void set_login(const service_login_request_type& login);

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] std::string get_surname() const;

        [[nodiscard]] std::string get_device_id() const;

        [[nodiscard]] std::string get_app_version() const;

        [[nodiscard]] int get_name_change_count() const;

        [[nodiscard]] void_awaitable_type time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login) override;

        [[nodiscard]] void_awaitable_type send_initial_sync() override;

        void send_role_response(int rpc = 0);

    private:
        using optional_user_role = std::optional<user_role>;
        using optional_user_server_roles = std::optional<user_server_roles>;
        using optional_server_role = std::optional<server_role>;

        [[nodiscard]] void_awaitable_type load_user_role_db();

        [[nodiscard]] void_awaitable_type load_user_server_roles_db();

        void set_server_role();

        optional_user_role user_role_;
        optional_user_server_roles user_server_roles_;
        int server_role_index_;
        optional_server_role server_role_;
        std::string device_id_;
        std::string app_version_;
    };
}