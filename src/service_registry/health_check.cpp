#include "health_check.h"
#include "common/celeritas_error.h"

#include <boost/json.hpp>

using namespace std::literals;

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

void celeritas::health_check::set_instance_id(const std::string& instance_id)
{
    instance_id_ = instance_id;
}

void celeritas::health_check::set_health_check_level(const health_check_level_type health_check_level)
{
    health_check_level_ = health_check_level;
}

std::string celeritas::health_check::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::health_check celeritas::health_check::from_json_string(const std::string& json_string)
{
    try
    {
        return do_from_json_string(json_string);
    }
    catch (const std::exception& e)
    {
        throw celeritas_error{ "json deserialization failed: "s + e.what() };
    }
}

celeritas::health_check celeritas::health_check::tag_invoke(const json_value& value)
{
    const auto& object = value.as_object();
    auto instance_id = boost::json::value_to<std::string>(object.at(instance_id_description));
    const auto health_check_level = boost::json::value_to<int>(object.at(health_check_level_description));

    return health_check{ std::move(instance_id), static_cast<health_check_level_type>(health_check_level) };
}

celeritas::health_check celeritas::health_check::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<health_check>(value);
}

celeritas::health_check celeritas::tag_invoke(health_check_tag, const health_check::json_value& value)
{
    try
    {
        return health_check::tag_invoke(value);
    }
    catch (const std::out_of_range& error)
    {
        throw celeritas_error{ "json deserialization failed: missing 'instance_id' or 'health_check_level' key."s + error.what() };
    }
    catch (const boost::system::system_error& error)
    {
        throw celeritas_error{ "json deserialization failed: invalid value type for key."s + error.what() };
    }
}

void celeritas::tag_invoke(boost::json::value_from_tag, health_check::json_value& value, const health_check& health_check)
{
    value = {
        { health_check::instance_id_description, health_check.get_instance_id() },
        { health_check::health_check_level_description, static_cast<int>(health_check.get_health_check_level()) }
    };
}
