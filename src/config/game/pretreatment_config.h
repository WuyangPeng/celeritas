#pragma once

#include "weight_config.h"

namespace celeritas
{
    class pretreatment_config
    {
    public:
        using class_type = pretreatment_config;
        using const_tables_shared_ptr = std::shared_ptr<const config::tables>;
        using const_weight_config_shared_ptr = std::shared_ptr<const weight_config>;

        explicit pretreatment_config(const const_tables_shared_ptr& tables);

        [[nodiscard]] const_weight_config_shared_ptr get_weight_config() const;

    private:
        const_weight_config_shared_ptr weight_config_;
    };
}