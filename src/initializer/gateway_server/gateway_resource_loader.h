#pragma once

#include "detail/player_check_timer.h"
#include "initializer/loader_base/resource_loader.h"

namespace celeritas
{
    class gateway_resource_loader final : public resource_loader
    {
    public:
        using class_type = gateway_resource_loader;
        using base_type = resource_loader;

        explicit gateway_resource_loader(std::string_view server_type, const_app_config_shared_ptr app_config);

        void send_player_check();

    private:
        using player_check_timer_shared_ptr = std::shared_ptr<player_check_timer>;

        void service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback) override;

        void start_player_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback);

        player_check_timer_shared_ptr player_check_timer_;
    };
}
