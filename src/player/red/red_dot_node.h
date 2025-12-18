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

        explicit red_dot_node(red_type type);

    private:
        using red_dot_node_weak_ptr = std::weak_ptr<red_dot_node>;
        using container = std::vector<red_dot_node_weak_ptr>;

        red_type type_;
        int value_ = 0;
        red_dot_node_weak_ptr parent_;
        container children_;
    };
}