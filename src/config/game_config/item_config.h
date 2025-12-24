#pragma once

#include "config/luban/generated/schema.h"

namespace celeritas
{
    class item_config
    {
    public:
        using class_type = item_config;
        using item_type = config::item_type;

        item_config(int item_template_id, item_type item_type, int stacked, bool squares);

        [[nodiscard]] int get_id() const;

        [[nodiscard]] int get_item_template_id() const;

        [[nodiscard]] item_type get_item_type() const;

        [[nodiscard]] int get_stacked() const;

        [[nodiscard]] bool is_squares() const;

    private:
        int item_template_id_ = 0;
        item_type item_type_ = item_type::none;
        int stacked_ = 0;
        bool squares_ = false;
    };
}