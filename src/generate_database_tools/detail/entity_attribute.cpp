#include "entity_attribute.h"

celeritas::entity_attribute::entity_attribute(std::string entity_name, std::string data_type, optional_string index_type)
    : entity_name_{ std::move(entity_name) }, data_type_{ std::move(data_type) }, index_type_{ std::move(index_type) }
{
}

std::string celeritas::entity_attribute::get_entity_name() const
{
    return entity_name_;
}

std::string celeritas::entity_attribute::get_data_type() const
{
    return data_type_;
}

celeritas::entity_attribute::optional_string celeritas::entity_attribute::get_index_type() const
{
    return index_type_;
}