#include "weight.h"
#include "common/core/random_helper.h"

void celeritas::weight::clear() noexcept
{
    id_.clear();
    weights_.clear();
}

void celeritas::weight::add_element(int id, double weights)
{
    id_.emplace_back(id);
    weights_.emplace_back(weights);
}

int celeritas::weight::get_random_index_by_weight() const
{
    return random_helper::get_random_index_by_weight(weights_);
}

int celeritas::weight::get_id(const int index) const
{
    return id_.at(index);
}

int celeritas::weight::get_random_id_by_weight() const
{
    const auto index = get_random_index_by_weight();

    return get_id(index);
}