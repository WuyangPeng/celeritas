#pragma once

#include <cstdint>

namespace celeritas
{
    class develop_level_data_key
    {
    public:
        using class_type = develop_level_data_key;

        develop_level_data_key(int develop_id, int level);

        [[nodiscard]] int get_develop_id() const;

        [[nodiscard]] int get_level() const;

    private:
        int develop_id_ = 0;
        int level_ = 0;
    };

    [[nodiscard]] bool operator<(const develop_level_data_key& lhs, const develop_level_data_key& rhs);
}