#pragma once

#include "detail/player_red_dot_database.h"
#include "detail/player_red_dot_document.h"
#include "detail/player_red_dot_node.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

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

        void add_red_dot(red_dot_type red_dot_type);

        void add_red_dot(red_dot_type red_dot_type, int value);

        void reduce_red_dot(red_dot_type red_dot_type);

        void reduce_red_dot(red_dot_type red_dot_type, int value);

        void change_red_dot(red_dot_type red_dot_type);

        [[nodiscard]] int get_red_dot_value(red_dot_type red_dot_type) const;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

    private:
        [[nodiscard]] void_awaitable_type load_user_red_dots();

        void set_red_dots();

        void calculate_red_dot();

        void update_document();

        player_red_dot_database database_;
        player_red_dot_document document_;
        player_red_dot_node node_;
    };
}