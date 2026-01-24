#pragma once

#include "http_base_message_handler.h"

template <typename HttpServiceType>
void celeritas::http_base_message_handler::co_spawn_response(http_handle_parameter_shared_ptr handle_parameter, const std::string_view channel_name, const std::string& error_message)
{
    co_spawn(handle_parameter->get_any_io_executor(),
             noexcept_safe_call_and_log_awaitable([handle_parameter = handle_parameter,
                                                      channel_name = channel_name,
                                                      error_message = error_message] {
                                                      return response<HttpServiceType>(handle_parameter, channel_name, error_message);
                                                  },
                                                  channel_name,
                                                  error_message),

             boost::asio::detached);
}

template <typename HttpServiceType>
celeritas::http_base_message_handler::void_awaitable_type celeritas::http_base_message_handler::response(http_handle_parameter_shared_ptr handle_parameter, const std::string_view channel_name, const std::string& error_message)
{
    auto http_service = std::make_shared<HttpServiceType>(std::move(handle_parameter));

    co_await noexcept_safe_call_and_log_awaitable([http_service = http_service] {
                                                      return http_service->response();
                                                  },
                                                  channel_name,
                                                  error_message);

    co_await http_service->send_error_response();
}