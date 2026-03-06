#pragma once

#include "message/handler_base/concrete_message_handler.h"
#include "player/component/player_state.h"

namespace celeritas
{
    template <typename Message>
    class player_concrete_message_handler : public concrete_message_handler<Message>
    {
    public:
        using class_type = player_concrete_message_handler;
        using base_type = concrete_message_handler<Message>;

    protected:
        using message_type = Message;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;
        using player_state_shared_ptr = std::shared_ptr<player_state>;

        template <typename ServiceType, typename... Args>
        void player_co_spawn_response(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                      const message_type& current_message,
                                      std::string_view channel_name,
                                      std::format_string<Args...> format,
                                      Args... args);

        template <typename ServiceType, typename... Args>
        [[nodiscard]] static void_awaitable_type player_response(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                                 const player_state_shared_ptr& player_state,
                                                                 const message_type& current_message,
                                                                 std::string_view channel_name,
                                                                 std::format_string<Args...> format,
                                                                 Args... args);
    };
}