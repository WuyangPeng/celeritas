#pragma once

#include "config/config_fwd.h"

#include <string>

namespace celeritas
{
    class red_config
    {
    public:
        using class_type = red_config;

        red_config(red_dot_type id, std::string name, red_dot_type parent_node_id, red_dot_status_type red_dot_status_type);

        [[nodiscard]] red_dot_type get_id() const;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] red_dot_type get_parent_node_id() const;

        [[nodiscard]] red_dot_status_type get_red_dot_status_type() const;

    private:
        red_dot_type id_;
        std::string name_;
        red_dot_type parent_node_id_;
        red_dot_status_type red_dot_status_type_;
    };
}