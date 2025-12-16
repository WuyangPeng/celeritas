#include "player_time_component.h"
#include "common/time_helper.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      document_{},
      database_{ player_state, this, &document_ },
      scheduler_{ player_state, this, &document_ }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    document_.set_document(co_await database_.load());
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_dependencies_ready()
{
    co_await on_all_time_callback(true);

    scheduler_.calculate_next_refresh_time();
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
    co_await scheduler_.on_time_callback([this] {
        on_data_change();
    });
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter, const bool is_login)
{
    co_await scheduler_.on_time_callback(time_refresh_type,
                                         parameter,
                                         is_login,
                                         [this] {
                                             on_data_change();
                                         });
}

void celeritas::player_time_component::register_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    scheduler_.register_timer(player_component,
                              time_refresh_type,
                              parameter,
                              [this] {
                                  on_data_change();
                              });
}

void celeritas::player_time_component::remove_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    scheduler_.remove_timer(player_component,
                            time_refresh_type,
                            parameter,
                            [this] {
                                on_data_change();
                            });
}

void celeritas::player_time_component::on_data_change()
{
    database_.update_document();
}

celeritas::player_time_component::void_awaitable_type celeritas::player_time_component::on_all_time_callback(const bool is_login)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    auto change = false;
    for (auto& element : document_.get_player_time_refresh_container() | std::views::values)
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
