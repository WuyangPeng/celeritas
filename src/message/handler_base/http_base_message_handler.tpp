#pragma once

#include "http_base_message_handler.h"

template <typename HttpServiceType, typename... Args>
void celeritas::http_base_message_handler::co_spawn_response(http_handle_parameter_shared_ptr handle_parameter, 
                                                              const std::string_view channel_name, 
                                                              std::format_string<Args...> format,
                                                              Args... args)
{
    safe_co_spawn(handle_parameter->get_any_io_executor(),
                  [handle_parameter, channel_name, format, args...] {
                      return response<HttpServiceType>(handle_parameter, channel_name, format, args...);
                  },
                  channel_name,
                  format,
                  args...);
}

template <typename HttpServiceType, typename... Args>
celeritas::http_base_message_handler::void_awaitable_type celeritas::http_base_message_handler::response(http_handle_parameter_shared_ptr handle_parameter, 
                                                                                                          const std::string_view channel_name, 
                                                                                                          std::format_string<Args...> format,
                                                                                                          Args... args)
{
    auto http_service = std::make_shared<HttpServiceType>(std::move(handle_parameter));

    if (!co_await noexcept_safe_call_and_log_awaitable([http_service = http_service]() -> boost::asio::awaitable<bool> {
                                                           co_await http_service->response();
                                                           co_return true;
                                                       },
                                                       channel_name,
                                                       false,
                                                       format,
                                                       args...))
    {
        co_await http_service->send_error_response();
    }
}