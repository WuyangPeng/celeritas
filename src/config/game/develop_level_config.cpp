#include "develop_level_config.h"
#include "common/core/enum_cast.h"

celeritas::develop_level_config::develop_level_config(const develop_level_config_container& container)
    : container_{}
{
    init_develop_level(container);
}

celeritas::develop_level_config::const_optional_develop_level_config_shared_ptr celeritas::develop_level_config::get_develop_level(const develop_level_data_key& key) const
{
    if (const auto iter = container_.find(key);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

void celeritas::develop_level_config::init_develop_level(const develop_level_config_container& container)
{
    for (const auto& element : container.getDataList())
    {
        container_.emplace(develop_level_data_key{ element->developId, element->level }, element);
    }
}

