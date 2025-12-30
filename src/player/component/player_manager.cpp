#include "player_manager.h"
#include "player_state.h"
#include "player_state_type.h"
#include "common/core/celeritas_error.h"
#include "common/core/time_helper.h"
#include "player/component/player_state.tpp"
#include "player/online/player_online_component.h"
#include "player/time/player_time_refresh_key.h"

#include <ranges>

celeritas::player_manager& celeritas::player_manager::get_instance()
{
    static player_manager manager;

    return manager;
}

celeritas::player_manager::player_state_shared_ptr celeritas::player_manager::add_player(const user& user,
                                                                                         const resource_loader_shared_ptr& resource_loader,
                                                                                         io_context_type& io_context,
                                                                                         const std::string& instance_id,
                                                                                         const service_login_request_type& login)
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
            iter->second->set_login(login);
            return iter->second;
        }
    }

    return container_.emplace(user.get_user_id(), std::make_shared<player_state>(user, resource_loader, io_context, instance_id, login)).first->second;
}

celeritas::player_manager::player_state_shared_ptr celeritas::player_manager::get_player(const int64_t user_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = container_.find(user_id);
        iter != container_.cend() && iter->second->get_player_state_type() == player_state_type::online)
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
        co_await element->time_callback(player_time_refresh_key{ time_refresh_type, parameter }, false);
    }
}

celeritas::player_manager::void_awaitable_type celeritas::player_manager::offline_player(const int64_t user_id)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = container_.find(user_id);
        iter != container_.cend())
    {
        co_await iter->second->on_logout();
    }
}

celeritas::player_manager::void_awaitable_type celeritas::player_manager::check_player()
{
    std::lock_guard lock{ mutex_ };

    std::vector<player_state_shared_ptr> remove_player{};
    for (const auto& element : container_ | std::views::values)
    {
        const auto heartbeat = element->get_component<player_online_component>()->get_heartbeat();
        const auto current_time = time_helper::get_current_milliseconds();
        switch (element->get_player_state_type())
        {
            case player_state_type::loading:
            case player_state_type::online:
            {
                if (current_time - heartbeat >= minute_milliseconds * 5)
                {
                    element->set_player_state_type(player_state_type::disconnected_ghost);
                    co_await element->on_logout();
                }
            }
            case player_state_type::disconnected_ghost:
            {
                if (current_time - heartbeat >= minute_milliseconds * 10)
                {
                    element->set_player_state_type(player_state_type::logout_pending);
                }
            }
            break;
            case player_state_type::logout_pending:
            {
                remove_player.emplace_back(element);
            }
            break;
        }
    }

    for (const auto& element : remove_player)
    {
        container_.erase(element->get_user_id());
    }

    co_return;
}

celeritas::player_manager::player_manager()
    : container_{}, mutex_{}
{
}