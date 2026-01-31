#include "item_container.h"

#include <algorithm>

void celeritas::item_container::add_item_info(const int template_id, const int64_t count)
{
    const auto iter = std::ranges::find_if(container_, [template_id](const auto& element) {
        return element.get_template_id() == template_id;
    });

    if (iter != container_.cend())
    {
        iter->add_count(count);
    }
    else
    {
        container_.emplace_back(template_id, count);
    }
}

celeritas::item_container::container_type_const_iter celeritas::item_container::begin() const
{
    return container_.cbegin();
}

celeritas::item_container::container_type_const_iter celeritas::item_container::end() const
{
    return container_.cend();
}