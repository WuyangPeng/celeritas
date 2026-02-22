#pragma once

#include "config/luban/generated/schema.h"

namespace celeritas
{
    class develop_system_key
    {
    public:
        using class_type = develop_system_key;

        develop_system_key(config::develop_system_type develop_system, config::develop_sub_type develop_sub);

        [[nodiscard]] config::develop_system_type get_develop_system() const;

        [[nodiscard]] config::develop_sub_type get_develop_sub() const;

    private:
        config::develop_system_type develop_system_;
        config::develop_sub_type develop_sub_;
    };

    [[nodiscard]] bool operator<(const develop_system_key& lhs, const develop_system_key& rhs);
}