#include "container_config.tpp"
#include "game_tables.h"
#include "pretreatment_config.h"

#include <ranges>

celeritas::game_tables::game_tables(const_tables_shared_ptr tables)
    : tables_{ std::move(tables) },
      pretreatment_config_{ std::make_shared<pretreatment_config>(tables_) }
{
}

celeritas::game_tables::const_tables_shared_ptr celeritas::game_tables::get_tables() const
{
    return tables_;
}

celeritas::game_tables::const_pretreatment_config_shared_ptr celeritas::game_tables::get_pretreatment_config() const
{
    return pretreatment_config_;
}
