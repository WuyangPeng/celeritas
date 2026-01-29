#include "item_selected_key.h"

celeritas::item_selected_key::item_selected_key(const item_type item_type, const config::item_selected_child_type child_type, const int64_t operation_id, const int parameter)
    : item_type_{ item_type }, child_type_{ child_type }, operation_id_{ operation_id }, parameter_{ parameter }
{
}

celeritas::item_selected_key::item_type celeritas::item_selected_key::get_item_type() const
{
    return item_type_;
}

celeritas::config::item_selected_child_type celeritas::item_selected_key::get_child_type() const
{
    return child_type_;
}

int64_t celeritas::item_selected_key::get_operation_id() const
{
    return operation_id_;
}

int celeritas::item_selected_key::get_parameter() const
{
    return parameter_;
}

bool celeritas::operator<(const item_selected_key& lhs, const item_selected_key& rhs)
{
    if (lhs.get_item_type() < rhs.get_item_type())
        return true;

    if (lhs.get_item_type() > rhs.get_item_type())
        return false;

    if (lhs.get_child_type() < rhs.get_child_type())
        return true;

    if (lhs.get_child_type() > rhs.get_child_type())
        return false;

    if (lhs.get_operation_id() < rhs.get_operation_id())
        return true;

    if (lhs.get_operation_id() > rhs.get_operation_id())
        return false;

    return lhs.get_parameter() < rhs.get_parameter();
}