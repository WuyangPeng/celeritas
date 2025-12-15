#include "player_resource_loader.h"
#include "database/config/config_manager.h"
#include "detail/player_server_fwd.h"

celeritas::player_resource_loader::player_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, player_state_check_timer_{}
{
}

void celeritas::player_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    start_health_check_timer(io_context, network_message_callback);
    load_database_config(io_context);
}

void celeritas::player_resource_loader::start_health_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    player_state_check_timer_ = std::make_unique<player_state_check_timer>(io_context, player_state_check_seconds);

    player_state_check_timer_->start();
}

void celeritas::player_resource_loader::load_database_config(io_context_type& io_context)
{
    config_manager::get_instance().load_from_db(io_context);
}