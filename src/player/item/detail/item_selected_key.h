#pragma once

#include "config/luban/generated/schema.h"

namespace celeritas
{
    class item_selected_key
    {
    public:
        using class_type = item_selected_key;
        using item_type = config::item_type;

        item_selected_key(item_type item_type, config::item_selected_child_type child_type, int64_t operation_id, int parameter);

        [[nodiscard]] item_type get_item_type() const;

        [[nodiscard]] config::item_selected_child_type get_child_type() const;

        [[nodiscard]] int64_t get_operation_id() const;

        [[nodiscard]] int get_parameter() const;

    private:
        item_type item_type_;
        config::item_selected_child_type child_type_;
        int64_t operation_id_;
        int parameter_;
    };

    [[nodiscard]] bool operator<(const item_selected_key& lhs, const item_selected_key& rhs);
}