#pragma once

#include "config/config_fwd.h"

#include <memory>
#include <vector>

namespace celeritas
{
    class red_dot_node
    {
    public:
        using class_type = red_dot_node;
        using red_dot_node_shared_ptr = std::shared_ptr<red_dot_node>;

        explicit red_dot_node(red_dot_type type);

        void set_value(int value);

        void set_parent_node(const red_dot_node_shared_ptr& parent_node);

        void add_child(const red_dot_node_shared_ptr& child);

        [[nodiscard]] bool is_child() const;

    private:
        using red_dot_node_weak_ptr = std::weak_ptr<red_dot_node>;
        using container = std::vector<red_dot_node_weak_ptr>;

        red_dot_type type_;
        int value_ = 0;
        red_dot_node_weak_ptr parent_;
        container children_;
    };
}