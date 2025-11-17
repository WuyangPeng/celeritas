#include "database_attribute.h"
#include "common/celeritas_error.h"

std::string celeritas::database_attribute::get_class_name() const
{
    return class_name_;
}

std::string celeritas::database_attribute::get_database_name() const
{
    return database_name_;
}

celeritas::database_attribute::entity_type celeritas::database_attribute::get_entity() const
{
    return entity_;
}

void celeritas::database_attribute::set_class_name(const std::string& class_name)
{
    class_name_ = class_name;
}

void celeritas::database_attribute::set_database_name(const std::string& database_name)
{
    database_name_ = database_name;
}

void celeritas::database_attribute::set_entity(const entity_type& entity)
{
    entity_ = entity;
}

celeritas::database_attribute celeritas::tag_invoke(boost::json::value_to_tag<database_attribute>, boost::json::value const& value)
{
    const auto& object = value.as_object();
    database_attribute attribute{};

    // 必填字段: class_name
    if (object.contains("class_name"))
    {
        attribute.set_class_name(boost::json::value_to<std::string>(object.at("class_name")));
    }
    else
    {
        throw celeritas_error("ClassDefinition missing 'class_name'");
    }

    // 必填字段: database_name
    if (object.contains("database_name"))
    {
        attribute.set_database_name(boost::json::value_to<std::string>(object.at("database_name")));
    }
    else
    {
        throw celeritas_error("ClassDefinition missing 'database_name'");
    }

    // 必填字段: entity (递归转换 vector)
    if (object.contains("entity"))
    {
        attribute.set_entity(boost::json::value_to<std::vector<entity_attribute> >(object.at("entity")));
    }
    else
    {
        throw celeritas_error("ClassDefinition missing 'entity'");
    }

    return attribute;
}

