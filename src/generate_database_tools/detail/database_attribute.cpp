#include "database_attribute.h"

celeritas::database_attribute::database_attribute(std::string class_name, std::string database_name, entity_type entity)
    : class_name_{ std::move(class_name) }, database_name_{ std::move(database_name) }, entity_{ std::move(entity) }
{
}

std::string celeritas::database_attribute::get_database_name() const
{
    return database_name_;
}

celeritas::database_attribute::entity_type celeritas::database_attribute::get_entity() const
{
    return entity_;
}

std::string celeritas::database_attribute::get_class_name() const
{
    return class_name_;
}