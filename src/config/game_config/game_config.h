#pragma once

#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"

#include <memory>
#include <shared_mutex>

namespace celeritas
{
    class game_config
    {
    public:
        using class_type = game_config;
        using const_game_tables_shared_ptr = std::shared_ptr<const game_tables>;
        using const_tables_shared_ptr = std::shared_ptr<const config::Tables>;

        [[nodiscard]] static game_config& get_instance();

        [[nodiscard]] const_game_tables_shared_ptr get_game_tables();

        void set_game_tables(const const_game_tables_shared_ptr& game_tables);

        [[nodiscard]] const_tables_shared_ptr get_tables();

        void set_tables(const const_tables_shared_ptr& tables);

    private:
        game_config();

        std::shared_mutex shared_mutex_;
        const_tables_shared_ptr tables_;
        const_game_tables_shared_ptr game_tables_;
    };
}