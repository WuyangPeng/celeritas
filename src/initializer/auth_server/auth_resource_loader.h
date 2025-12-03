#pragma once

#include "common/timer_base.h"
#include "detail/gateway_check_timer.h"
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

    private:
        using gateway_check_timer_shared_ptr = std::shared_ptr<gateway_check_timer>;
        using void_waitable_type = boost::asio::awaitable<void>;
        using http_client_shared_ptr = std::shared_ptr<http_client>;

        void service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback) override;

        static void load_from_db(io_context_type& io_context);

        void start_gateway_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        gateway_check_timer_shared_ptr gateway_check_timer_;
    };
}
