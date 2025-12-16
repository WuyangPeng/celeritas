#include "player_manager.h"
#include "player_state.h"
#include "player_state_type.h"
#include "common/celeritas_error.h"

#include <ranges>

celeritas::player_manager& celeritas::player_manager::get_instance()
{
    static player_manager manager;

    return manager;
}

celeritas::player_manager::player_state_shared_ptr celeritas::player_manager::add_player(const user& user, const resource_loader_shared_ptr& resource_loader, const std::string& instance_id)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = container_.find(user.get_user_id());
        iter != container_.cend())
    {
        if (const auto player_state_type = iter->second->get_player_state_type();
            player_state_type == player_state_type::loading ||
            player_state_type == player_state_type::logout_pending)
        {
            container_.erase(user.get_user_id());
        }
        else
        {
            iter->second->set_player_state_type(player_state_type::online);
            iter->second->set_instance_id(instance_id);
            return iter->second;
        }
    }

    return container_.emplace(user.get_user_id(), std::make_shared<player_state>(user, resource_loader, instance_id)).first->second;
}

celeritas::player_manager::player_state_shared_ptr celeritas::player_manager::get_player(const int64_t user_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = container_.find(user_id);
        iter != container_.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "player is no exist,user id = {}", user_id };
}

celeritas::player_manager::void_awaitable_type celeritas::player_manager::save_db()
{
    std::shared_lock lock{ mutex_ };

    for (const auto& element : container_ | std::views::values)
    {
        co_await element->save_db();
    }
}

celeritas::player_manager::void_awaitable_type celeritas::player_manager::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter)
{
    std::shared_lock lock{ mutex_ };

    for (const auto& element : container_ | std::views::values)
    {
        co_await element->time_callback(time_refresh_type, parameter, false);
    }
}

celeritas::player_manager::player_manager()
    : container_{}, mutex_{}
{
}