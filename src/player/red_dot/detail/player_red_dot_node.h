#pragma once

#include "player_red_dot_document.h"
#include "config/game_config/red_dot_type.h"
#include "player/red_dot/red_dot_node.h"

#include <map>

namespace celeritas
{
    class player_red_dot_node
    {
    public:
        using class_type = player_red_dot_node;

        explicit player_red_dot_node(player_state* player_state, player_red_dot_document* player_red_dot_document);

        [[nodiscard]] bool add_red_dot(red_dot_type red_dot_type, int value);

        [[nodiscard]] bool reduce_red_dot(red_dot_type red_dot_type, int value);

        [[nodiscard]] bool change_red_dot(red_dot_type red_dot_type);

        void set_red_dot_node();

        [[nodiscard]] bool calculate_red_dot();

    private:
        using red_dot_node_shared_ptr = std::shared_ptr<red_dot_node>;
        using red_dot_node_container = std::map<red_dot_type, red_dot_node_shared_ptr>;

        player_state* player_state_;
        player_red_dot_document* player_red_dot_document_;
        red_dot_node_container red_dot_node_;
    };
}