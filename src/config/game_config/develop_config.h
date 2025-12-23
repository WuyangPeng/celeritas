#pragma once

#include "develop_reset_type.h"
#include "develop_sub_type.h"
#include "develop_system_type.h"

namespace celeritas
{
    class develop_config
    {
    public:
        using class_type = develop_config;

        develop_config(int id, develop_system_type develop_system_type, develop_sub_type develop_sub_type, int max_level, develop_reset_type develop_reset_type);

        [[nodiscard]] int get_id() const;

        [[nodiscard]] develop_system_type get_develop_system_type() const;

        [[nodiscard]] develop_sub_type get_develop_sub_type() const;

        [[nodiscard]] int get_max_level() const;

        [[nodiscard]] develop_reset_type get_develop_reset_type() const;

    private:
        int id_ = 0;
        develop_system_type develop_system_type_ = develop_system_type::null;
        develop_sub_type develop_sub_type_ = develop_sub_type::null;
        int max_level_ = 0;
        develop_reset_type develop_reset_type_ = develop_reset_type::non_resettable;
    };
}