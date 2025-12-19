#include "equipment_data.h"

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>

celeritas::equipment_data::equipment_data()
    : strength_{ 0 }, durability_{ 0 }
{
}

celeritas::equipment_data::equipment_data(const document_view_type& document_view)
    : strength_{ 0 }, durability_{ 0 }
{
    set_document(document_view);
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

celeritas::equipment_data::document_type celeritas::equipment_data::to_document_type() const
{
    document_type builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ strength_description }, strength_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ durability_description }, durability_));
    return builder;
}

void celeritas::equipment_data::set_document(const document_view_type& document_view)
{
    if (const auto strength_element = document_view[strength_description];
        strength_element && strength_element.type() == bsoncxx::type::k_int32)
    {
        strength_ = strength_element.get_int32().value;
    }

    if (const auto durability_element = document_view[durability_description];
        durability_element && durability_element.type() == bsoncxx::type::k_int32)
    {
        durability_ = durability_element.get_int32().value;
    }
}
