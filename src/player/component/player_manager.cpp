#include "player_manager.h"
#include "player_state.h"
#include "player_state_type.h"
#include "common/celeritas_error.h"

celeritas::player_manager& celeritas::player_manager::get_instance()
{
    static player_manager manager;

    return manager;
}

celeritas::player_manager::player_state_shared_ptr celeritas::player_manager::add_player(const int64_t user_id, const std::string& game_server_id, const resource_loader_shared_ptr& resource_loader)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = container_.find(user_id);
        iter != container_.cend())
    {
        if (const auto player_state_type = iter->second->get_player_state_type();
            player_state_type == player_state_type::loading ||
            player_state_type == player_state_type::logout_pending)
        {
            container_.erase(user_id);
        }
        else
        {
            iter->second->set_player_state_type(player_state_type::online);
            return iter->second;
        }
    }

    return container_.emplace(user_id, std::make_shared<player_state>(user_id, game_server_id, resource_loader)).first->second;
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

celeritas::player_manager::player_manager()
    : container_{}, mutex_{}
{
}