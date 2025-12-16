#include "player_time_document.h"
#include "change_timer_result.h"
#include "common/time_helper.h"
#include "player/component/player_state.h"

#include <ranges>

celeritas::player_time_document::player_time_document(player_state* player_state)
    : player_state_{ player_state }
{
}

const celeritas::player_time_document::player_time_refresh_container& celeritas::player_time_document::get_player_time_refresh_container() const
{
    return player_time_refresh_;
}

celeritas::player_time_document::player_time_refresh_container& celeritas::player_time_document::get_player_time_refresh_container()
{
    return player_time_refresh_;
}

void celeritas::player_time_document::set_document(const player_time_refresh_container& player_time_refresh)
{
    player_time_refresh_ = player_time_refresh;
}

int64_t celeritas::player_time_document::calculate_next_refresh_time() const
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

celeritas::change_timer_result celeritas::player_time_document::register_timer(const player_component_type component_type, const time_refresh_type refresh_type, const int64_t parameter, const int64_t time_id)
{
    if (const auto iter = player_time_refresh_.find(player_time_refresh_key{ refresh_type, parameter, time_id });
        iter != player_time_refresh_.end())
    {
        auto& element = iter->second;
        const auto component = element.get_component();
        if (const auto component_iter = std::ranges::find(component, component_type);
            component_iter != component.cend())
        {
            return change_timer_result::no_change;
        }

        element.add_component(component_type);

        return change_timer_result::change_component;
    }

    player_time_refresh_.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(refresh_type, parameter, time_id),
                                 std::forward_as_tuple(refresh_type, parameter, time_id, component_type));

    return change_timer_result::change_document;
}

celeritas::change_timer_result celeritas::player_time_document::remove_timer(const player_component_type component_type, const time_refresh_type refresh_type, const int64_t parameter, const int64_t time_id)
{
    if (const auto iter = player_time_refresh_.find(player_time_refresh_key{ refresh_type, parameter, time_id });
        iter != player_time_refresh_.end())
    {
        auto& element = iter->second;
        element.remove_component(component_type);
        if (element.get_component().empty())
        {
            player_time_refresh_.erase(iter);
            return change_timer_result::change_document;
        }
        return change_timer_result::change_component;
    }

    return change_timer_result::no_change;
}

celeritas::player_time_document::change_timer_result_awaitable_type celeritas::player_time_document::on_time_callback(const bool is_login, const bool is_including_default)
{
    auto change_timer_result = change_timer_result::no_change;
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_ | std::views::values)
    {
        if (!is_including_default && element.is_default())
        {
            continue;
        }

        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await player_state_->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), element.get_time_id(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
            change_timer_result = change_timer_result::change_document;
        }
    }

    co_return change_timer_result;
}

celeritas::player_time_document::change_timer_result_awaitable_type celeritas::player_time_document::on_time_callback(time_refresh_type refresh_type, int64_t parameter, int64_t time_id, bool is_login)
{
    auto change_timer_result = change_timer_result::no_change;
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    if (const auto iter = player_time_refresh_.find(player_time_refresh_key{ refresh_type, parameter, time_id });
        iter != player_time_refresh_.end())
    {
        if (auto& element = iter->second;
            element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await player_state_->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), element.get_time_id(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
        }

        change_timer_result = change_timer_result::change_document;
    }

    co_return change_timer_result;
}