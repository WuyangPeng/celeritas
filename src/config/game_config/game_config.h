#pragma once

#include "config/config_fwd.h"

#include <memory>
#include <shared_mutex>

namespace celeritas
{
    class game_config
    {
    public:
        using class_type = game_config;
        using const_game_tables_shared_ptr = std::shared_ptr<const game_tables>;

        [[nodiscard]] static game_config& get_instance();

        [[nodiscard]] const_game_tables_shared_ptr get_game_tables();

        void set_game_tables(const const_game_tables_shared_ptr& game_tables);

    private:
        game_config();

        std::shared_mutex shared_mutex_;
        const_game_tables_shared_ptr game_tables_;
    };
}