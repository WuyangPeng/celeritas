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
        using const_tables_shared_ptr = std::shared_ptr<const config::tables>;
        using sex_type = config::sex_type;
        using red_dot_type = config::red_dot_type;

        explicit game_tables(const_tables_shared_ptr tables);

        [[nodiscard]] std::string get_surname() const;

        [[nodiscard]] std::string get_name(sex_type sex_type) const;

        [[nodiscard]] const_tables_shared_ptr get_tables() const;

    private:
        using name_weight_type = std::map<sex_type, weight>;

        void init_surname_config();

        void init_name_config();

        const_tables_shared_ptr tables_;

        weight surname_weight_;
        name_weight_type name_weight_;
    };
}