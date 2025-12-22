#include "item_container.h"

void celeritas::item_container::add_item_info(const int template_id, const int64_t count)
{
    container_.emplace_back(template_id, count);
}

celeritas::item_container::container_type_const_iter celeritas::item_container::begin() const
{
    return container_.cbegin();
}

celeritas::item_container::container_type_const_iter celeritas::item_container::end() const
{
    return container_.cend();
}