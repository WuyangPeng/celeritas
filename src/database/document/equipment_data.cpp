#include "equipment_data.h"

celeritas::equipment_data::equipment_data()
    : strength_{ 0 }, durability_{ 0 }
{
}

celeritas::equipment_data::equipment_data(const int strength, const int durability)
    : strength_{ strength }, durability_{ durability }
{
}

int celeritas::equipment_data::get_strength() const
{
    return strength_;
}

void celeritas::equipment_data::set_strength(const int strength)
{
    strength_ = strength;
}

int celeritas::equipment_data::get_durability() const
{
    return durability_;
}

void celeritas::equipment_data::set_durability(const int durability)
{
    durability_ = durability;
}

