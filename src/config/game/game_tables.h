#pragma once

#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"

#include <memory>

namespace celeritas
{
    class game_tables
    {
    public:
        using class_type = game_tables;
        using const_tables_shared_ptr = std::shared_ptr<const config::tables>;
        using const_pretreatment_config_shared_ptr = std::shared_ptr<const pretreatment_config>;

        explicit game_tables(const_tables_shared_ptr tables);

        [[nodiscard]] const_tables_shared_ptr get_tables() const;

        [[nodiscard]] const_pretreatment_config_shared_ptr get_pretreatment_config() const;

    private:
        const_tables_shared_ptr tables_;
        const_pretreatment_config_shared_ptr pretreatment_config_;
    };
}