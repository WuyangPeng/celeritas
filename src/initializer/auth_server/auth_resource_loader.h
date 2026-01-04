#pragma once

#include "detail/gateway_check_timer.h"
#include "detail/auth_health_check_timer.h"
#include "initializer/resource_loader.h"

namespace celeritas
{
    class auth_resource_loader final : public resource_loader
    {
    public:
        using class_type = auth_resource_loader;
        using base_type = resource_loader;

        explicit auth_resource_loader(std::string_view server_type, app_config_shared_ptr app_config);

        void send_gateway_check();

        void send_health_check(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback);

    private:
        using gateway_check_timer_shared_ptr = std::shared_ptr<gateway_check_timer>;
        using health_check_timer_shared_ptr = std::shared_ptr<auth_health_check_timer>;
        using void_waitable_type = boost::asio::awaitable<void>;
        using http_client_shared_ptr = std::shared_ptr<http_client>;

        void service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback) override;

        static void load_from_db(const any_io_executor& any_io_executor);

        void start_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback);

        void start_gateway_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback);

        void start_health_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback);

        [[nodiscard]] void_waitable_type send_health_check(http_client_shared_ptr http_client);

        gateway_check_timer_shared_ptr gateway_check_timer_;
        health_check_timer_shared_ptr health_check_timer_;
    };
}
