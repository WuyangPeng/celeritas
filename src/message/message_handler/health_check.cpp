#include "health_check.h"
#include "common/celeritas_error.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

celeritas::health_check::health_check(std::string instance_id, const bool is_healthy)
    : instance_id_{ std::move(instance_id) }, is_healthy_{ is_healthy }
{
}

std::string celeritas::health_check::get_instance_id() const
{
    return instance_id_;
}

bool celeritas::health_check::is_healthy() const
{
    return is_healthy_;
}

std::string celeritas::health_check::to_json_string() const
{
    boost::property_tree::ptree tree{};
    tree.put("instance_id", instance_id_);
    tree.put("is_healthy", is_healthy_);

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
        const auto instance_id = tree.get<std::string>("instance_id");
        const auto healthy = tree.get<bool>("is_healthy");

        return health_check(std::move(instance_id), healthy);
    }
    catch (const boost::property_tree::ptree_error& e)
    {
        throw celeritas_error("json deserialization failed: Missing or invalid key in json: " + std::string(e.what()));
    }
}