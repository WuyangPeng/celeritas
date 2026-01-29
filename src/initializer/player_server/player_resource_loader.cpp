#include "player_resource_loader.h"
#include "auth/config/server_cell_repository.h"
#include "common/core/time_helper.h"
#include "database/config/config_manager.h"
#include "detail/player_server_fwd.h"

#include <chrono>

celeritas::player_resource_loader::player_resource_loader(const std::string_view server_type, const_app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, player_state_check_timer_{}, player_default_timer_{}
{
}

void celeritas::player_resource_loader::service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    load_from_db(any_io_executor);
    load_database_config(any_io_executor);
    start_health_check_timer(any_io_executor, network_message_callback);
    start_player_default_timer(any_io_executor, network_message_callback);
}

void celeritas::player_resource_loader::start_health_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    player_state_check_timer_ = std::make_unique<player_state_check_timer>(any_io_executor, player_state_check_seconds);

    player_state_check_timer_->start();
}

void celeritas::player_resource_loader::start_player_default_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto now = std::chrono::system_clock::now();

    const auto current_hour_time_point = floor<std::chrono::hours>(now);

    const auto next_hour_time_point = current_hour_time_point + std::chrono::hours(1);

    const auto duration_to_next_hour = next_hour_time_point - now;

    const auto milliseconds_to_next_hour = duration_cast<std::chrono::milliseconds>(duration_to_next_hour);

    const auto current_zone = time_helper::get_local_zone();

    const auto zone_time = std::chrono::zoned_time{ current_zone, next_hour_time_point };

    const auto local_time = zone_time.get_local_time();

    const auto local_day = floor<std::chrono::days>(local_time);

    const auto time_of_day_duration = local_time - local_day;

    const auto local_hours = duration_cast<std::chrono::hours>(time_of_day_duration).count();

    player_default_timer_ = std::make_unique<player_default_timer>(any_io_executor, milliseconds_to_next_hour, local_hours);

    player_default_timer_->start();
}

void celeritas::player_resource_loader::load_database_config(const any_io_executor& any_io_executor)
{
    config_manager::get_instance().load_from_db(any_io_executor);
}

void celeritas::player_resource_loader::load_from_db(const any_io_executor& any_io_executor)
{
    server_cell_repository::get_instance().load_from_db(any_io_executor);
}