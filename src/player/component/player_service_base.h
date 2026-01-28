#pragma once

#include "player_state.h"
#include "message/parameters/protobuf_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class player_service_base
    {
    public:
        using class_type = player_service_base;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using any_io_executor = boost::asio::any_io_executor;
        using io_context_type = boost::asio::io_context;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;
        using player_state_shared_ptr = std::shared_ptr<player_state>;

        explicit player_service_base(protobuf_handle_parameter_shared_ptr handle_parameter,
                                     player_state_shared_ptr player_state);

        virtual ~player_service_base() noexcept = default;

        player_service_base(const player_service_base& rhs) = delete;

        player_service_base& operator=(const player_service_base& rhs) = delete;

        player_service_base(player_service_base&& rhs) noexcept = delete;

        player_service_base& operator=(player_service_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type response() = 0;

    protected:
        using const_game_tables_shared_ptr = std::shared_ptr<const game_tables>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        [[nodiscard]] player_state_shared_ptr get_player_state();

        [[nodiscard]] int get_rpc() const;

        [[nodiscard]] const_game_tables_shared_ptr get_game_tables() const;

        [[nodiscard]] const_app_config_shared_ptr get_config() const;

    private:
        protobuf_handle_parameter_shared_ptr handle_parameter_;
        player_state_shared_ptr player_state_;
        const_game_tables_shared_ptr game_tables_;
    };
}
