#pragma once

#include "develop_level_config.h"
#include "rename_cost_config.h"
#include "weight_config.h"

namespace celeritas
{
    class pretreatment_config
    {
    public:
        using class_type = pretreatment_config;
        using const_tables_shared_ptr = std::shared_ptr<const config::tables>;
        using const_weight_config_shared_ptr = std::shared_ptr<const weight_config>;
        using const_rename_cost_config_shared_ptr = std::shared_ptr<const rename_cost_config>;
        using const_develop_level_config_shared_ptr = std::shared_ptr<const develop_level_config>;

        explicit pretreatment_config(const const_tables_shared_ptr& tables);

        [[nodiscard]] const_weight_config_shared_ptr get_weight_config() const;

        [[nodiscard]] const_rename_cost_config_shared_ptr get_rename_cost_config() const;

        [[nodiscard]] const_develop_level_config_shared_ptr get_develop_level_config() const;

    private:
        const_weight_config_shared_ptr weight_config_;
        const_rename_cost_config_shared_ptr rename_cost_config_;
        const_develop_level_config_shared_ptr develop_level_config_;
    };
}