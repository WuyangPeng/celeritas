#include "pretreatment_config.h"

celeritas::pretreatment_config::pretreatment_config(const const_tables_shared_ptr& tables)
    : weight_config_{ std::make_shared<weight_config>(tables->surname_config_container, tables->name_config_container) },
      rename_cost_config_{ std::make_shared<rename_cost_config>(tables->rename_cost_config_container) },
      develop_level_config_{ std::make_shared<develop_level_config>(tables->develop_level_config_container) }
{
}

celeritas::pretreatment_config::const_weight_config_shared_ptr celeritas::pretreatment_config::get_weight_config() const
{
    return weight_config_;
}

celeritas::pretreatment_config::const_rename_cost_config_shared_ptr celeritas::pretreatment_config::get_rename_cost_config() const
{
    return rename_cost_config_;
}

celeritas::pretreatment_config::const_develop_level_config_shared_ptr celeritas::pretreatment_config::get_develop_level_config() const
{
    return develop_level_config_;
}