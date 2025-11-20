#pragma once

#include "message/http_base_message_handler.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class health_check_request_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = health_check_request_http_message_handler;
        using base_type = http_base_message_handler;

        explicit health_check_request_http_message_handler(std::string path);

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] void_awaitable_type health_check_result(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type do_health_check_result(http_handle_parameter handle_parameter);

        std::string path_;
    };
}