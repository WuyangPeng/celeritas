#include "building_data.h"
#include "database/basic/basis_database.tpp"

celeritas::building_data::building_data()
    : level_{ 0 }
{
}

celeritas::building_data::building_data(const int level)
    : level_{ level }
{
}

int celeritas::building_data::get_level() const
{
    return level_;
}

void celeritas::building_data::set_level(const int level)
{
    level_ = level;
}

celeritas::building_data::document_type celeritas::building_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(level_description, level_);

    return document;
}

celeritas::building_data celeritas::building_data::from_document(const document_type& document)
{
    building_data building_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == level_description)
        {
            building_data.set_level(element.get_value<database_data_type::int32_type>());
        }
    }

    return building_data;
}
