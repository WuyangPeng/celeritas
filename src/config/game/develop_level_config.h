#pragma once

#include "develop_level_data_key.h"
#include "config/luban/generated/schema.h"

#include <map>

namespace celeritas
{
    class develop_level_config
    {
    public:
        using class_type = develop_level_config;
        using develop_level_config_container = config::game::develop_level_config_container;
        using const_develop_level_config_shared_ptr = luban::SharedPtr<const config::game::develop_level_config>;
        using const_optional_develop_level_config_shared_ptr = std::optional<const_develop_level_config_shared_ptr>;

        explicit develop_level_config(const develop_level_config_container& container);

        [[nodiscard]] const_optional_develop_level_config_shared_ptr get_develop_level(const develop_level_data_key& key) const;

    private:
        using container_type = std::map<develop_level_data_key, const_develop_level_config_shared_ptr>;

        void init_develop_level(const develop_level_config_container& container);

        container_type container_;
    };
}