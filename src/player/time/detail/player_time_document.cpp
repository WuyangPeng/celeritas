#include "player_time_document.h"
#include "common/time_helper.h"

#include <ranges>

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

bool celeritas::player_time_document::register_timer(player_component_type component_type, time_refresh_type refresh_type, int64_t parameter)
{
    if (const auto iter = player_time_refresh_.find(player_time_refresh_key{ refresh_type, parameter });
        iter != player_time_refresh_.end())
    {
        auto& element = iter->second;
        const auto component = element.get_component();
        if (const auto component_iter = std::ranges::find(component, component_type);
            component_iter != component.cend())
        {
            return false;
        }

        element.add_component(component_type);
        return true;
    }

    player_time_refresh_.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(refresh_type, parameter),
                                 std::forward_as_tuple(refresh_type, parameter, component_type));
    return true;
}