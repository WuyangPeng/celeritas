#pragma once

#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"

#include <string>

namespace celeritas
{
    class red_dot_config
    {
    public:
        using class_type = red_dot_config;
        using red_dot_type = config::red_dot_type;

        red_dot_config(red_dot_type id, std::string name, red_dot_type parent_node_id, red_dot_status_type red_dot_status_type, bool save_database);

        [[nodiscard]] red_dot_type get_id() const;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] red_dot_type get_parent_node_id() const;

        [[nodiscard]] red_dot_status_type get_red_dot_status_type() const;

        [[nodiscard]] bool is_save_database() const;

    private:
        red_dot_type id_;
        std::string name_;
        red_dot_type parent_node_id_;
        red_dot_status_type red_dot_status_type_;
        bool save_database_;
    };
}