#pragma once

#include "weight.h"
#include "config/config_fwd.h"

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
        using name_config_config = container_config<name_config>;
        using const_name_config_shared_ptr = std::shared_ptr<const name_config_config>;

        game_tables();

        [[nodiscard]] const_surname_container_config_shared_ptr get_surname_config() const;

        void set_surname_config(const const_surname_container_config_shared_ptr& surname_config);

        [[nodiscard]] const_name_config_shared_ptr get_name_config() const;

        void set_name_config(const const_name_config_shared_ptr& name_config);

        [[nodiscard]] std::string get_surname() const;

        [[nodiscard]] std::string get_name(sex_type sex_type) const;

    private:
        using name_weight_type = std::map<sex_type, weight>;

        const_surname_container_config_shared_ptr surname_config_;
        const_name_config_shared_ptr name_config_;
        weight surname_weight_;
        name_weight_type name_weight_;
    };
}