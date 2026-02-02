#pragma once

#include <cstdint>

namespace celeritas
{
    class develop_level_data_key
    {
    public:
        using class_type = develop_level_data_key;

        develop_level_data_key(int system_id, int64_t instance_id, int level);

        [[nodiscard]] int get_system_id() const;

        [[nodiscard]] int64_t get_instance_id() const;

        [[nodiscard]] int get_level() const;

    private:
        int system_id_ = 0;
        int64_t instance_id_ = 0;
        int level_ = 0;
    };

    [[nodiscard]] bool operator<(const develop_level_data_key& lhs, const develop_level_data_key& rhs);
}