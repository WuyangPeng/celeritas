#pragma once

#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"

#include <memory>
#include <optional>
#include <vector>

namespace celeritas
{
    class red_dot_node
    {
    public:
        using class_type = red_dot_node;
        using red_dot_type = config::red_dot_type;
        using red_dot_node_shared_ptr = std::shared_ptr<red_dot_node>;
        using red_dot_node_optional = std::optional<red_dot_node_shared_ptr>;

        explicit red_dot_node(red_dot_type type, bool save_database);

        void set_value(int value);

        void add_value(int value);

        void reduce_value(int value);

        [[nodiscard]] int get_value() const;

        void set_parent_node(const red_dot_node_shared_ptr& parent_node);

        void add_child(const red_dot_node_shared_ptr& child);

        [[nodiscard]] bool is_child() const;

        [[nodiscard]] bool is_save_database() const;

        [[nodiscard]] red_dot_node_optional get_parent() const;

        [[nodiscard]] red_dot_type get_red_dot_type() const;

    private:
        using red_dot_node_weak_ptr = std::weak_ptr<red_dot_node>;
        using container = std::vector<red_dot_node_weak_ptr>;

        void check_value();

        red_dot_type type_;
        int value_ = 0;
        red_dot_node_weak_ptr parent_;
        container children_;
        bool save_database_ = false;
    };
}