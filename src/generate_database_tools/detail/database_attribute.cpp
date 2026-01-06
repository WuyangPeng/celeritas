#include "database_attribute.h"
#include "common/core/celeritas_error.h"
#include "database/entity/entity.h"

celeritas::database_attribute::database_attribute()
    : class_name_{},
      database_name_{},
      entity_{},
      key_type_{},
      key_name_{}
{
}

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

std::string celeritas::database_attribute::get_key_type() const
{
    return key_type_;
}

std::string celeritas::database_attribute::get_key_name() const
{
    return key_name_;
}

celeritas::database_attribute::entity_const_iterator celeritas::database_attribute::begin() const
{
    return entity_.cbegin();
}

celeritas::database_attribute::entity_const_iterator celeritas::database_attribute::end() const
{
    return entity_.cend();
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

    key_type_.clear();
    key_name_.clear();

    for (const auto& attribute : entity_)
    {
        if (const auto& index_type = attribute.get_index_type();
            index_type.has_value() &&
            *index_type == "key")
        {
            key_type_ = attribute.get_data_type();
            key_name_ = attribute.get_entity_name();
            break;
        }
    }
}

int celeritas::database_attribute::size() const
{
    return entity_.size();
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

