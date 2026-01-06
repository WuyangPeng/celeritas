#include "equipment_data.h"
#include "database/basic/basis_database.tpp"

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>

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

celeritas::equipment_data::document_type celeritas::equipment_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(strength_description, strength_);
    document.emplace_back(durability_description, durability_);

    return document;
}

celeritas::equipment_data celeritas::equipment_data::from_document(const document_type& document)
{
    equipment_data equipment_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == strength_description)
        {
            equipment_data.set_strength(element.get_value<database_data_type::int32_type>());
        }
        if (element.get_field_name() == durability_description)
        {
            equipment_data.set_durability(element.get_value<database_data_type::int32_type>());
        }
    }

    return equipment_data;
}
