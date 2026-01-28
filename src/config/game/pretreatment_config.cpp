#include "pretreatment_config.h"

celeritas::pretreatment_config::pretreatment_config(const const_tables_shared_ptr& tables)
    : weight_config_{ std::make_shared<weight_config>(tables->surname_config_container, tables->name_config_container) }
{
}

celeritas::pretreatment_config::const_weight_config_shared_ptr celeritas::pretreatment_config::get_weight_config() const
{
    return weight_config_;
}