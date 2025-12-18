#pragma once

#include "database/document/red_dots.h"
#include "database/generated/mongo/player/user_red_dots.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

#include <map>

namespace celeritas
{
    class player_red_dot_component final : public player_component
    {
    public:
        using class_type = player_red_dot_component;
        using base_type = player_component;

        explicit player_red_dot_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::red_dot;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type on_dependencies_ready() override;

    private:
        using optional_user_red_dots = std::optional<user_red_dots>;
        using red_dots_container = std::map<red_type, red_dots>;
        using red_dot_node_shared_ptr = std::shared_ptr<red_dot_node>;
        using red_dot_node_container = std::map<red_type, red_dot_node_shared_ptr>;

        [[nodiscard]] void_awaitable_type load_user_red_dots();

        void set_red_dots();

        void set_red_dot_node();

        void calculate_red_dot_by_database();

        void calculate_children_red_dot();

        void calculate_parent_red_dot();

        optional_user_red_dots user_red_dots_;
        red_dots_container red_dots_;
        red_dot_node_container red_dot_node_;
    };
}