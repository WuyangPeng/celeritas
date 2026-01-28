#include "item_selected_key.h"

celeritas::item_selected_key::item_selected_key(item_type item_type, int child_type)
    : item_type_{ item_type }, child_type_{ child_type }
{
}

celeritas::item_selected_key::item_type celeritas::item_selected_key::get_item_type() const
{
    return item_type_;
}

int celeritas::item_selected_key::get_child_type() const
{
    return child_type_;
}

bool celeritas::operator<(const item_selected_key& lhs, const item_selected_key& rhs)
{
    if (lhs.get_item_type() < rhs.get_item_type())
        return true;

    if (lhs.get_item_type() > rhs.get_item_type())
        return false;

    return lhs.get_child_type() < rhs.get_child_type();
}