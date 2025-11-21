#pragma once

#include "detail/health_check_timer.h"
#include "initializer/resource_loader.h"

namespace celeritas
{
    class service_registry_resource_loader final : public resource_loader
    {
    public:
        using class_type = service_registry_resource_loader;
        using base_type = resource_loader;

        explicit service_registry_resource_loader(std::string_view server_type, app_config_shared_ptr app_config);

        void send_health_check(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

    private:
        using health_check_timer_shared_ptr = std::shared_ptr<health_check_timer>;
        using void_waitable_type = boost::asio::awaitable<void>;
        using http_client_shared_ptr = std::shared_ptr<http_client>;

        void service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback) override;

        void start_health_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        [[nodiscard]] void_waitable_type send_health_check(http_client_shared_ptr http_client);

        health_check_timer_shared_ptr health_check_timer_;
    };
}
