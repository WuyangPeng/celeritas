#pragma once

#include "player_red_dot_document.h"
#include "config/game/container_config.h"
#include "config/luban/generated/schema.h"
#include "player/red_dot/red_dot_node.h"

#include <map>

namespace celeritas
{
    class player_red_dot_node
    {
    public:
        using class_type = player_red_dot_node;
        using red_dot_type = config::red_dot_type;

        explicit player_red_dot_node(player_state* player_state, player_red_dot_document* player_red_dot_document);

        bool add_red_dot(red_dot_type red_dot_type, int value) const;

        [[nodiscard]] bool reduce_red_dot(red_dot_type red_dot_type, int value) const;

        [[nodiscard]] bool change_red_dot(red_dot_type red_dot_type) const;

        void set_red_dot_node();

        [[nodiscard]] bool calculate_red_dot();

        [[nodiscard]] int get_red_dot_value(red_dot_type red_dot_type) const;

    private:
        using red_dot_node_shared_ptr = std::shared_ptr<red_dot_node>;
        using red_dot_node_container = std::map<red_dot_type, red_dot_node_shared_ptr>;
        using red_container_type = container_config<config::container::red_dot_config, red_dot_type>::container;
        using const_red_dot_config_shared_ptr = std::shared_ptr<const config::container::red_dot_config>;

        [[nodiscard]] red_dot_node_shared_ptr get_child_red_dot(red_dot_type red_dot_type) const;

        [[nodiscard]] bool insert_document(const red_dot_node_shared_ptr& red_dot_node) const;

        [[nodiscard]] bool set_document(const red_dot_node_shared_ptr& red_dot_node) const;

        static void add_parent_value(const red_dot_node_shared_ptr& red_dot_node, int value);

        void init_red_dot_node(const config::container::red_dot_config_container& container);

        void set_red_dot_node_association(const config::container::red_dot_config_container& container);

        void set_red_dot_node_association(const const_red_dot_config_shared_ptr& red_dot_config);

        [[nodiscard]] bool calculate_red_dot(const red_dot_node_shared_ptr& red_dot_node) const;

        [[nodiscard]] bool calculate_red_dot_by_save_database(const red_dot_node_shared_ptr& red_dot_node) const;

        [[nodiscard]] int get_red_dot_value(const red_dot_node_shared_ptr& red_dot_node) const;

        player_state* player_state_;
        player_red_dot_document* player_red_dot_document_;
        red_dot_node_container red_dot_node_;
    };
}