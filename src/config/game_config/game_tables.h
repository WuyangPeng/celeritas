#pragma once

#include "weight.h"
#include "config/config_fwd.h"
#include "config/game_config/container_config.h"
#include "config/luban/generated/schema.h"

#include <map>
#include <memory>

namespace celeritas
{
    class game_tables
    {
    public:
        using class_type = game_tables;
        using const_tables_shared_ptr = std::shared_ptr<const config::Tables>;
        using sex_type = config::sex_type;
        using red_dot_type = config::red_dot_type;
        using red_dot_container_config = container_config<red_dot_config, red_dot_type>;
        using const_red_dot_container_shared_ptr = std::shared_ptr<const red_dot_container_config>;
        using develop_container_config = container_config<develop_config>;
        using const_develop_container_shared_ptr = std::shared_ptr<const develop_container_config>;

        explicit game_tables(const_tables_shared_ptr tables);

        [[nodiscard]] std::string get_surname() const;

        [[nodiscard]] std::string get_name(sex_type sex_type) const;

        [[nodiscard]] const_red_dot_container_shared_ptr get_red_dot_config() const;

        void set_red_dot_config(const const_red_dot_container_shared_ptr& red_dot);

        [[nodiscard]] const_develop_container_shared_ptr get_develop_config() const;

        void set_develop_config(const const_develop_container_shared_ptr& develop);

        [[nodiscard]] const_tables_shared_ptr get_tables() const;

    private:
        using name_weight_type = std::map<sex_type, weight>;

        void init_surname_config();

        void init_name_config();

        const_tables_shared_ptr tables_;

        weight surname_weight_;
        name_weight_type name_weight_;

        const_red_dot_container_shared_ptr red_dot_;

        const_develop_container_shared_ptr develop_;
    };
}