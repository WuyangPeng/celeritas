#include "rename_cost_config.h"
#include "common/core/celeritas_error.h"

celeritas::rename_cost_config::rename_cost_config(const rename_cost_config_container& container)
    : container_{}
{
    init_rename_cost(container);
}

const celeritas::rename_cost_config::priority_item_container& celeritas::rename_cost_config::get_priority_item(int id) const
{
    if (const auto iter = container_.find(id);
        iter != container_.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "rename cost config is not find,id = {}", id };
}

void celeritas::rename_cost_config::init_rename_cost(const rename_cost_config_container& container)
{
    for (const auto& element : container.getDataList())
    {
        auto item = element->item;
        std::ranges::sort(item, [](const auto& lhs, const auto& rhs) {
            return lhs->priority < rhs->priority;
        });

        container_.emplace(element->id, item);
    }
}