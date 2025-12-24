#pragma once

#include "config/luban/generated/schema.h"

namespace celeritas
{
    class develop_config
    {
    public:
        using class_type = develop_config;
        using develop_system_type = config::develop_system_type;
        using develop_sub_type = config::develop_sub_type;
        using develop_reset_type = config::develop_reset_type;

        develop_config(int id, develop_system_type develop_system_type, develop_sub_type develop_sub_type, int max_level, develop_reset_type develop_reset_type);

        [[nodiscard]] int get_id() const;

        [[nodiscard]] develop_system_type get_develop_system_type() const;

        [[nodiscard]] develop_sub_type get_develop_sub_type() const;

        [[nodiscard]] int get_max_level() const;

        [[nodiscard]] develop_reset_type get_develop_reset_type() const;

    private:
        int id_ = 0;
        develop_system_type develop_system_type_ = develop_system_type::none;
        develop_sub_type develop_sub_type_ = develop_sub_type::none;
        int max_level_ = 0;
        develop_reset_type develop_reset_type_ = develop_reset_type::non_resettable;
    };
}