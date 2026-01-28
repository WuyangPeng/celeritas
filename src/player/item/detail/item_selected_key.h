#pragma once

#include "config/luban/generated/schema.h"

namespace celeritas
{
    class item_selected_key
    {
    public:
        using class_type = item_selected_key;
        using item_type = config::item_type;

        item_selected_key(item_type item_type, int child_type);

        [[nodiscard]] item_type get_item_type() const;

        [[nodiscard]] int get_child_type() const;

    private:
        item_type item_type_;
        int child_type_;
    };

    [[nodiscard]] bool operator<(const item_selected_key& lhs, const item_selected_key& rhs);
}