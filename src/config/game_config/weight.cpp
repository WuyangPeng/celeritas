#include "weight.h"

void celeritas::weight::clear()
{
    id_.clear();
    weights_.clear();
}

void celeritas::weight::add_element(int id, double weights)
{
    id_.emplace_back(id);
    weights_.emplace_back(weights);
}

const celeritas::weight::weights_type& celeritas::weight::get_weights() const
{
    return weights_;
}

int celeritas::weight::get_id(const int index) const
{
    return id_.at(index);
}