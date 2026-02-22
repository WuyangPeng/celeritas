#include "develop_config.h"
#include "common/core/enum_cast.h"

celeritas::develop_config::develop_config(const develop_config_container& container)
    : container_{}
{
    init_develop(container);
}

celeritas::develop_config::const_optional_develop_config_shared_ptr celeritas::develop_config::get_develop(const develop_system_key& key) const
{
    if (const auto iter = container_.find(key);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

void celeritas::develop_config::init_develop(const develop_config_container& container)
{
    for (const auto& element : container.getDataList())
    {
        container_.emplace(develop_system_key{ element->developSystemType, element->developSubType }, element);
    }
}

