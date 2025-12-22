#include "item_info.h"

celeritas::item_info::item_info(const int template_id, const int64_t count)
    : template_id_{ template_id }, count_{ count }
{
}

int celeritas::item_info::get_template_id() const
{
    return template_id_;
}

int64_t celeritas::item_info::get_count() const
{
    return count_;
}