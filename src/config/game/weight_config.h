#pragma once

#include "weight.h"
#include "config/luban/generated/schema.h"

#include <map>

namespace celeritas
{
    class weight_config
    {
    public:
        using class_type = weight_config;
        using sex_type = config::sex_type;
        using surname_config_container_type = config::game::surname_config_container;
        using name_config_container_type = config::game::name_config_container;

        weight_config(const surname_config_container_type& surname_config_container,
                      const name_config_container_type& name_config_container);

        [[nodiscard]] std::string get_surname(const surname_config_container_type& surname_config_container) const;

        [[nodiscard]] std::string get_name(sex_type sex_type, const name_config_container_type& name_config_container) const;

    private:
        using name_weight_type = std::map<sex_type, weight>;
        using name_config_type = config::game::name_config;

        void init_surname_config(const surname_config_container_type& surname_config_container);

        void init_name_config(const name_config_container_type& name_config_container);

        void init_name_config(const name_config_type& config);

        weight surname_weight_;
        name_weight_type name_weight_;
    };
}