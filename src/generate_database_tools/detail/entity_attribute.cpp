#include "entity_attribute.h"
#include "common/celeritas_error.h"

std::string celeritas::entity_attribute::get_entity_name() const
{
    return entity_name_;
}

std::string celeritas::entity_attribute::get_data_type() const
{
    return data_type_ + "_type";
}

celeritas::entity_attribute::optional_string celeritas::entity_attribute::get_index_type() const
{
    return index_type_;
}

void celeritas::entity_attribute::set_entity_name(const std::string& entityName)
{
    entity_name_ = entityName;
}

void celeritas::entity_attribute::set_data_type(const std::string& dataType)
{
    data_type_ = dataType;
}

void celeritas::entity_attribute::set_index_type(const std::string& indexType)
{
    index_type_ = indexType;
}

celeritas::entity_attribute celeritas::tag_invoke(boost::json::value_to_tag<entity_attribute>, boost::json::value const& value)
{
    const auto& object = value.as_object();
    entity_attribute attribute{};

    if (object.contains("entity_name"))
    {
        attribute.set_entity_name(boost::json::value_to<std::string>(object.at("entity_name")));
    }
    else
    {
        throw celeritas_error("entity_attribute missing 'entity_name'");
    }

    if (object.contains("data_type"))
    {
        attribute.set_data_type(boost::json::value_to<std::string>(object.at("data_type")));
    }
    else
    {
        throw celeritas_error("entity_attribute missing 'data_type'");
    }

    if (object.contains("index_type"))
    {
        attribute.set_index_type(boost::json::value_to<std::string>(object.at("index_type")));
    }

    return attribute;
}