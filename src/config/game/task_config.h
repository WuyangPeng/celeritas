#pragma once

#include "config/luban/generated/schema.h"

#include <map>
#include <vector>
#include <memory>

namespace celeritas
{
    class task_config
    {
    public:
        using class_type = task_config;
        using task_config_container = config::game::task_config_container;
        using const_task_config_shared_ptr = luban::SharedPtr<const config::game::task_config>;
        using task_config_shared_ptr_container = std::vector<const_task_config_shared_ptr>;
        using element_type = std::shared_ptr<task_config_shared_ptr_container>;
        using optional_element_type = std::optional<element_type>;

        explicit task_config(const task_config_container& container);

        [[nodiscard]] optional_element_type get_tasks_by_type(config::task_component_type type) const;

    private:
        using container_type = std::map<config::task_component_type, element_type>;

        void init_tasks(const task_config_container& container);

        container_type container_;
    };
}