#include "player_time_component.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_time_refresh_{}, player_time_refresh_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    const auto mongo_pool = get_mongo_player_db_name();
    const auto user_id = get_player_state()->get_user_id();

    if (const auto optional_user_time_refresh = co_await mongo_pool->select_one(user_time_refresh::get_select(database_type::mongo, user_id), user_time_refresh::get_database_field_container()))
    {
        user_time_refresh_ = user_time_refresh{ *optional_user_time_refresh };
    }
    else
    {
        user_time_refresh_ = user_time_refresh{ database_type::mongo, user_id };
    }

    for (const auto& element : user_time_refresh_->get_player_time_refresh())
    {
        player_time_refresh_.emplace_back(player_time_refresh::from_json_string(element));
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_dependencies_ready()
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), true);
            }

            element.set_last_refresh_time(current_milliseconds);
        }
    }
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::save_db()
{
    if (user_time_refresh_->is_must_save())
    {
        const auto mongo_pool = get_mongo_player_db_name();

        co_await mongo_pool->execute_changes(user_time_refresh_->get_modify());

        user_time_refresh_->clear_modify();
    }
}

bool celeritas::player_time_component::is_modify() const
{
    return user_time_refresh_->is_must_save();
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter, const bool is_login)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.get_time_refresh_type() == time_refresh_type &&
            element.get_parameter() == parameter &&
            element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
        }
    }
    co_return;
}

void celeritas::player_time_component::register_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    for (auto& element : player_time_refresh_)
    {
        if (element.get_time_refresh_type() == time_refresh_type &&
            element.get_parameter() == parameter)
        {
            const auto component = element.get_component();
            if (const auto iter = std::ranges::find(component, player_component);
                iter != component.end())
            {
                return;
            }

            element.add_component(player_component);
            return;
        }
    }

    player_time_refresh_.emplace_back(time_refresh_type, parameter, player_component);
}

void celeritas::player_time_component::remove_timer(player_component_type player_component, time_refresh_type time_refresh_type, int64_t parameter)
{
    std::erase_if(player_time_refresh_,
                  [&](auto& element) {
                      if (element.get_time_refresh_type() == time_refresh_type &&
                          element.get_parameter() == parameter)
                      {
                          element.remove_component(player_component);
                          return element.get_component().empty();
                      }
                      return false;
                  });
}

int64_t celeritas::player_time_component::get_next_refresh_time() const
{
    auto result = 0LL;
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (const auto next_refresh_time = element.get_next_refresh_time();
            next_refresh_time > current_milliseconds &&
            (result == 0 || result > next_refresh_time))
        {
            result = next_refresh_time;
        }
    }
    return result;
}
