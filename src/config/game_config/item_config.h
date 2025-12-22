#pragma once

#include "item_type.h"

namespace celeritas
{
    class item_config
    {
    public:
        using class_type = item_config;

        item_config(int item_template_id, item_type item_type, int stacked, bool squares);

        [[nodiscard]] int get_item_template_id() const;

        [[nodiscard]] item_type get_item_type() const;

        [[nodiscard]] int get_stacked() const;

        [[nodiscard]] bool is_squares() const;

    private:
        int item_template_id_ = 0;
        item_type item_type_ = item_type::null;
        int stacked_ = 0;
        bool squares_ = false;
    };
}