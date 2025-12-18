#pragma once

#include "weight.h"
#include "config/config_fwd.h"
#include "config/game_config/container_config.h"

#include <map>
#include <memory>

namespace celeritas
{
    class game_tables
    {
    public:
        using class_type = game_tables;
        using surname_container_config = container_config<surname_config>;
        using const_surname_container_config_shared_ptr = std::shared_ptr<const surname_container_config>;
        using name_container_config = container_config<name_config>;
        using const_name_container_shared_ptr = std::shared_ptr<const name_container_config>;
        using red_container_config = container_config<red_config, red_type>;
        using const_red_container_shared_ptr = std::shared_ptr<const red_container_config>;

        game_tables();

        [[nodiscard]] const_surname_container_config_shared_ptr get_surname_config() const;

        void set_surname_config(const const_surname_container_config_shared_ptr& surname_config);

        [[nodiscard]] const_name_container_shared_ptr get_name_config() const;

        void set_name_config(const const_name_container_shared_ptr& name_config);

        [[nodiscard]] std::string get_surname() const;

        [[nodiscard]] std::string get_name(sex_type sex_type) const;

        [[nodiscard]] const_red_container_shared_ptr get_red_container() const;

        void set_red_container(const const_red_container_shared_ptr& redContainer);

    private:
        using name_weight_type = std::map<sex_type, weight>;

        const_surname_container_config_shared_ptr surname_config_;
        weight surname_weight_;

        const_name_container_shared_ptr name_config_;
        name_weight_type name_weight_;

        const_red_container_shared_ptr red_container_;
    };
}