#pragma once

#include "common/common_fwd.h"
#include "player/player_fwd.h"

#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class player_manager
    {
    public:
        using class_type = player_manager;
        using player_state_shared_ptr = std::shared_ptr<player_state>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;

        [[nodiscard]] static player_manager& get_instance();

        [[nodiscard]] player_state_shared_ptr add_player(int64_t user_id, const std::string& game_server_id, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] player_state_shared_ptr get_player(int64_t user_id);

    private:
        using container_type = std::unordered_map<int64_t, player_state_shared_ptr>;

        player_manager();

        container_type container_;
        std::shared_mutex mutex_;
    };
}