#include "develop_data.h"
#include "common/time_helper.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::develop_data::develop_data(const int system_id, const int64_t instance_id)
    : system_id_{ system_id }, instance_id_{ instance_id }, level_{ 1 }, exp_{ 0 }, updated_time_{ time_helper::get_current_milliseconds() }
{
}

int celeritas::develop_data::get_system_id() const
{
    return system_id_;
}

void celeritas::develop_data::set_system_id(const int systemId)
{
    system_id_ = systemId;
}

int64_t celeritas::develop_data::get_instance_id() const
{
    return instance_id_;
}

void celeritas::develop_data::set_instance_id(const int64_t instanceId)
{
    instance_id_ = instanceId;
}

int celeritas::develop_data::get_level() const
{
    return level_;
}

void celeritas::develop_data::set_level(const int level)
{
    level_ = level;
}

void celeritas::develop_data::add_level()
{
    ++level_;
    updated_time_ = time_helper::get_current_milliseconds();
}

int64_t celeritas::develop_data::get_exp() const
{
    return exp_;
}

void celeritas::develop_data::set_exp(const int64_t exp)
{
    exp_ = exp;
}

int64_t celeritas::develop_data::get_updated_time() const
{
    return updated_time_;
}

void celeritas::develop_data::set_updated_time(const int64_t updated_time)
{
    updated_time_ = updated_time;
}

void celeritas::develop_data::clear()
{
    level_ = 0;
    exp_ = 0;
    updated_time_ = time_helper::get_current_milliseconds();
}

std::string celeritas::develop_data::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ system_id_description }, system_id_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ instance_id_description }, instance_id_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ level_description }, level_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ exp_description }, exp_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ updated_time_description }, updated_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::develop_data celeritas::develop_data::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    develop_data develop_data{};

    develop_data.set_system_id(parsed_view[system_id_description].get_int32().value);
    develop_data.set_instance_id(parsed_view[instance_id_description].type() == bsoncxx::type::k_int32 ? parsed_view[instance_id_description].get_int32().value : parsed_view[instance_id_description].get_int64().value);
    develop_data.set_level(parsed_view[level_description].get_int32().value);
    develop_data.set_exp(parsed_view[exp_description].type() == bsoncxx::type::k_int32 ? parsed_view[exp_description].get_int32().value : parsed_view[exp_description].get_int64().value);
    develop_data.set_updated_time(parsed_view[updated_time_description].type() == bsoncxx::type::k_int32 ? parsed_view[updated_time_description].get_int32().value : parsed_view[updated_time_description].get_int64().value);

    return develop_data;
}