#include "player_time_component.h"
#include "common/time_helper.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      player_time_refresh_{},
      database_{ player_state, this },
      scheduler_{ player_state, this }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    player_time_refresh_ = co_await database_.load();
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_dependencies_ready()
{
    co_await on_all_time_callback(true);

    scheduler_.calculate_next_refresh_time(player_time_refresh_);
    scheduler_.init_player_timer();
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::save_db()
{
    co_await database_.save();
}

bool celeritas::player_time_component::is_modify() const
{
    return database_.is_must_save();
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback()
{
    co_await scheduler_.on_time_callback(player_time_refresh_, [this] {
        on_data_change();
    });
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter, const bool is_login)
{
    co_await scheduler_.on_time_callback(player_time_refresh_, time_refresh_type, parameter, is_login, [this] {
        on_data_change();
    });
}

void celeritas::player_time_component::register_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    scheduler_.register_timer(player_time_refresh_, player_component, time_refresh_type, parameter, [this] {
        on_data_change();
    });
}

void celeritas::player_time_component::remove_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    scheduler_.remove_timer(player_time_refresh_, player_component, time_refresh_type, parameter, [this] {
        on_data_change();
    });
}

int64_t celeritas::player_time_component::get_next_refresh_time() const
{
    return scheduler_.get_next_refresh_time();
}

int64_t celeritas::player_time_component::calculate_next_refresh_time() const
{
    auto result = 0LL;
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_ | std::views::values)
    {
        if (element.is_default())
        {
            continue;
        }
        if (const auto next_refresh_time = element.get_next_refresh_time();
            next_refresh_time > current_milliseconds &&
            (result == 0 || result > next_refresh_time))
        {
            result = next_refresh_time;
        }
    }

    return result;
}

void celeritas::player_time_component::on_data_change()
{
    database_.update_document(player_time_refresh_);
}

celeritas::player_time_component::void_awaitable_type celeritas::player_time_component::on_all_time_callback(bool is_login)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    auto change = false;
    for (auto& element : player_time_refresh_ | std::views::values)
    {
        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
            change = true;
        }
    }

    if (change)
    {
        on_data_change();
    }
}
