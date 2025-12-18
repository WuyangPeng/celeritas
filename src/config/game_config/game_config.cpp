#include "game_config.h"
#include "game_tables.h"

celeritas::game_config& celeritas::game_config::get_instance()
{
    static game_config instance{};

    return instance;
}

celeritas::game_config::const_game_tables_shared_ptr celeritas::game_config::get_game_tables()
{
    std::shared_lock lock{ shared_mutex_ };

    return game_tables_;
}

void celeritas::game_config::set_game_tables(const const_game_tables_shared_ptr& game_tables)
{
    std::lock_guard lock{ shared_mutex_ };

    game_tables_ = game_tables;
}

celeritas::game_config::game_config()
    : game_tables_{ std::make_shared<game_tables>() }
{
}