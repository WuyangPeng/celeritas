#pragma once

#include "player_concrete_message_handler.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

template <typename Message>
template <typename ServiceType>
void celeritas::player_concrete_message_handler<Message>::player_co_spawn_response(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                                   const message_type& current_message,
                                                                                   std::string_view channel_name,
                                                                                   const std::string& error_message)
{
    if (auto player = player_manager::get_instance().get_player(handle_parameter->get_user_id());
        player != nullptr)
    {
        co_spawn(player->get_any_io_executor(),
                 noexcept_safe_call_and_log_awaitable([handle_parameter = handle_parameter,
                                                          current_message = current_message,
                                                          channel_name = channel_name,
                                                          error_message = error_message,
                                                          player = player] {
                                                          return player_response<ServiceType>(handle_parameter, player, current_message, channel_name, error_message);
                                                      },
                                                      channel_name,
                                                      error_message),

                 boost::asio::detached);
    }
}

template <typename Message>
template <typename ServiceType>
celeritas::player_concrete_message_handler<Message>::void_awaitable_type celeritas::player_concrete_message_handler<Message>::player_response(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                                                                                                              const player_state_shared_ptr& player_state,
                                                                                                                                              const message_type& current_message,
                                                                                                                                              std::string_view channel_name,
                                                                                                                                              const std::string& error_message)
{
    const auto rpc = handle_parameter->get_rpc();
    if (auto service = ServiceType::create(std::move(handle_parameter), player_state, current_message);
        !co_await noexcept_safe_call_and_log_awaitable([service = service]() -> boost::asio::awaitable<bool> {
                                                           co_await service->response();
                                                           co_return true;
                                                       },
                                                       channel_name,
                                                       error_message,
                                                       false))
    {
        player_state->send_error_message(rpc, game_error_type::unknown);
    }
}