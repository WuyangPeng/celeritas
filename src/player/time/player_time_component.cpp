#include "player_time_component.h"
#include "detail/change_timer_result.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      document_{ player_state },
      database_{ player_state->get_user_id(), this, &document_ },
      scheduler_{ player_state, this, &document_ }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    document_.set_document(std::move(co_await database_.load()));
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

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter, const int64_t time_id, const bool is_login)
{
    co_await scheduler_.on_time_callback(time_refresh_type,
                                         parameter,
                                         time_id,
                                         is_login,
                                         [this] {
                                             on_data_change();
                                         });
}

void celeritas::player_time_component::register_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter, const int64_t time_id)
{
    scheduler_.register_timer(player_component,
                              time_refresh_type,
                              parameter,
                              time_id,
                              [this] {
                                  on_data_change();
                              });
}

void celeritas::player_time_component::remove_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter, const int64_t time_id)
{
    scheduler_.remove_timer(player_component,
                            time_refresh_type,
                            parameter,
                            time_id,
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
    if (const auto result = co_await document_.on_time_callback(is_login, true);
        result != change_timer_result::no_change)
    {
        on_data_change();
    }
}
