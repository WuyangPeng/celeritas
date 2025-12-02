#pragma once

#include "message/http_base_message_handler.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class notify_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = notify_http_message_handler;
        using base_type = http_base_message_handler;

        explicit notify_http_message_handler(int64_t sdk_id, std::string path_suffix);

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static void_awaitable_type response(http_handle_parameter handle_parameter);

        int64_t sdk_id_;
        std::string path_suffix_;
    };
}