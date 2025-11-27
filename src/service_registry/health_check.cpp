#include "health_check.h"
#include "common/celeritas_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

celeritas::health_check::health_check(std::string instance_id, const health_check_level_type health_check_level)
    : instance_id_{ std::move(instance_id) }, health_check_level_{ health_check_level }
{
}

std::string celeritas::health_check::get_instance_id() const
{
    return instance_id_;
}

celeritas::health_check_level_type celeritas::health_check::get_health_check_level() const
{
    return health_check_level_;
}

std::string celeritas::health_check::to_json_string() const
{
    boost::property_tree::ptree tree{};
    tree.put("instance_id", instance_id_);
    tree.put("health_check_level", static_cast<int>(health_check_level_));

    try
    {
        std::stringstream string_stream{};
        boost::property_tree::write_json(string_stream, tree, false);
        return string_stream.str();
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
        throw celeritas_error("json serialization failed: " + std::string(e.what()));
    }
}

celeritas::health_check celeritas::health_check::from_json_string(const std::string& json_string)
{
    std::stringstream string_stream{ json_string };
    boost::property_tree::ptree tree;

    try
    {
        boost::property_tree::read_json(string_stream, tree);
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
        throw celeritas_error("json deserialization failed: Invalid json format: " + std::string(e.what()));
    }

    try
    {
        auto instance_id = tree.get<std::string>("instance_id");
        const auto health_check_level = tree.get<int>("health_check_level");

        return health_check{ std::move(instance_id), static_cast<health_check_level_type>(health_check_level) };
    }
    catch (const boost::property_tree::ptree_error& e)
    {
        throw celeritas_error("json deserialization failed: Missing or invalid key in json: " + std::string(e.what()));
    }
}

void celeritas::health_check::set_instance_id(const std::string& instance_id)
{
    instance_id_ = instance_id;
}

void celeritas::health_check::set_health_check_level(const health_check_level_type health_check_level)
{
    health_check_level_ = health_check_level;
}

celeritas::health_check celeritas::tag_invoke(boost::json::value_to_tag<health_check>, boost::json::value const& value)
{
    const auto& object = value.as_object();
    health_check attribute{};

    // 必填字段: instance_id
    if (object.contains("instance_id"))
    {
        attribute.set_instance_id(boost::json::value_to<std::string>(object.at("instance_id")));
    }
    else
    {
        throw celeritas_error("ClassDefinition missing 'instance_id'");
    }

    // 必填字段: health_check_level
    if (object.contains("health_check_level"))
    {
        attribute.set_health_check_level(static_cast<health_check_level_type>(boost::json::value_to<int>(object.at("health_check_level"))));
    }
    else
    {
        throw celeritas_error("ClassDefinition missing 'health_check_level'");
    }

    return attribute;
}