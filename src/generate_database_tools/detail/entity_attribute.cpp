#include "entity_attribute.h"
#include "../../common/core/celeritas_error.h"

std::string celeritas::entity_attribute::get_entity_name() const
{
    return entity_name_;
}

std::string celeritas::entity_attribute::get_data_type() const
{
    return data_type_ + "_type";
}

std::string celeritas::entity_attribute::get_element_type() const
{
    return data_type_ + "_element_type";
}

celeritas::entity_attribute::optional_string celeritas::entity_attribute::get_index_type() const
{
    return index_type_;
}

celeritas::entity_attribute::optional_string celeritas::entity_attribute::get_default_value() const
{
    return default_value_;
}

bool celeritas::entity_attribute::is_bool_type() const
{
    return data_type_ == "bool";
}

bool celeritas::entity_attribute::is_string_type() const
{
    return data_type_ == "string";
}

bool celeritas::entity_attribute::is_noexcept_type() const
{
    return data_type_ == "int32" ||
           data_type_ == "int32_count" ||
           data_type_ == "int64" ||
           data_type_ == "int64_count" ||
           data_type_ == "double" ||
           data_type_ == "bool";
}

bool celeritas::entity_attribute::is_count_type() const
{
    return data_type_.find("_count") != std::string::npos;
}

bool celeritas::entity_attribute::is_key_type() const
{
    return index_type_.has_value() && index_type_ == "key";
}

bool celeritas::entity_attribute::is_array_type() const
{
    if (data_type_.find("array") != std::string::npos)
    {
        return true;
    }

    return false;
}

void celeritas::entity_attribute::set_entity_name(const std::string& entity_name)
{
    entity_name_ = entity_name;
}

void celeritas::entity_attribute::set_data_type(const std::string& data_type)
{
    data_type_ = data_type;
}

void celeritas::entity_attribute::set_index_type(const std::string& index_type)
{
    index_type_ = index_type;
}

void celeritas::entity_attribute::set_default_value(const std::string& default_value)
{
    default_value_ = default_value;
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

    if (object.contains("default_value"))
    {
        attribute.set_default_value(boost::json::value_to<std::string>(object.at("default_value")));
    }

    return attribute;
}