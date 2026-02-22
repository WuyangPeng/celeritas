#pragma once

#include "develop_data_key.h"
#include "develop_system_key.h"
#include "config/luban/generated/schema.h"

#include <map>

namespace celeritas
{
    class develop_config
    {
    public:
        using class_type = develop_config;
        using develop_config_container = config::game::develop_config_container;
        using const_develop_config_shared_ptr = luban::SharedPtr<const config::game::develop_config>;
        using const_optional_develop_config_shared_ptr = std::optional<const_develop_config_shared_ptr>;

        explicit develop_config(const develop_config_container& container);

        [[nodiscard]] const_optional_develop_config_shared_ptr get_develop(const develop_system_key& key) const;

    private:
        using container_type = std::map<develop_system_key, const_develop_config_shared_ptr>;

        void init_develop(const develop_config_container& container);

        container_type container_;
    };
}