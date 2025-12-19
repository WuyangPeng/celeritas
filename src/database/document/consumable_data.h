#pragma once

#include <cstdint>

namespace celeritas
{
    class consumable_data
    {
    public:
        using class_type = consumable_data;

        consumable_data();

        explicit consumable_data(int64_t expire_time);

        [[nodiscard]] int64_t get_expire_time() const;

        void set_expire_time(int64_t expire_time);

    private:
        int64_t expire_time_;
    };
}
