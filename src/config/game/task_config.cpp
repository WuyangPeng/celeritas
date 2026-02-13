#include "task_config.h"

celeritas::task_config::task_config(const task_config_container& container)
    : container_{}
{
    init_tasks(container);
}

celeritas::task_config::optional_element_type celeritas::task_config::get_tasks_by_type(const config::task_component_type type) const
{
    if (const auto iter = container_.find(type);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

void celeritas::task_config::init_tasks(const task_config_container& container)
{
    for (const auto& element : container.getDataList())
    {
        auto& component = container_[element->taskComponentType];
        if (!component)
        {
            component = std::make_shared<task_config_shared_ptr_container>();
        }
        component->emplace_back(element);
    }
}