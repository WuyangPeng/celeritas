#pragma once

#include "detail/player_state_check_timer.h"
#include "initializer/resource_loader.h"

namespace celeritas
{
    class player_resource_loader final : public resource_loader
    {
    public:
        using class_type = player_resource_loader;
        using base_type = resource_loader;

        explicit player_resource_loader(std::string_view server_type, app_config_shared_ptr app_config);

    private:
        using player_state_check_timer_shared_ptr = std::shared_ptr<player_state_check_timer>;

        void service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback) override;

        void start_health_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        player_state_check_timer_shared_ptr player_state_check_timer_;
    };
}
