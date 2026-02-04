#pragma once

#include <cstdint>

namespace celeritas
{
    class item_info
    {
    public:
        using class_type = item_info;

        item_info(int template_id, int64_t count);

        [[nodiscard]] int get_template_id() const;

        [[nodiscard]] int64_t get_count() const;

        void add_count(int64_t count);

        void proportion_count(int proportion);

    private:
        int template_id_;
        int64_t count_;
    };
}