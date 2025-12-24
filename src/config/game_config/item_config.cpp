#include "item_config.h"

celeritas::item_config::item_config(const int item_template_id, const item_type item_type, const int stacked, const bool squares)
    : item_template_id_{ item_template_id }, item_type_{ item_type }, stacked_{ stacked }, squares_{ squares }
{
}

int celeritas::item_config::get_id() const
{
    return item_template_id_;
}

int celeritas::item_config::get_item_template_id() const
{
    return item_template_id_;
}

celeritas::config::item_type celeritas::item_config::get_item_type() const
{
    return item_type_;
}

int celeritas::item_config::get_stacked() const
{
    return stacked_;
}

bool celeritas::item_config::is_squares() const
{
    return squares_;
}