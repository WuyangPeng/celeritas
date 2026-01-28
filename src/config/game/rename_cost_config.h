#pragma once

#include "config/luban/generated/schema.h"

#include  <map>

namespace celeritas
{
    class rename_cost_config
    {
    public:
        using class_type = rename_cost_config;
        using rename_cost_config_container = config::game::rename_cost_config_container;
        using priority_item_shared_ptr = luban::SharedPtr<config::priority_item>;
        using priority_item_container = std::vector<priority_item_shared_ptr>;

        explicit rename_cost_config(const rename_cost_config_container& container);

        [[nodiscard]] const priority_item_container& get_priority_item(int id) const;

    private:
        using container_type = std::map<int, priority_item_container>;

        void init_rename_cost(const rename_cost_config_container& container);

        container_type container_;
    };
}