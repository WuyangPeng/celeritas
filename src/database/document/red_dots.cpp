#include "red_dots.h"
#include "common/time_helper.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::red_dots::red_dots()
    : node_id_{}, state_{}, last_value_{}, update_time_{}
{
}

celeritas::red_dots::red_dots(const red_dot_type node_id, const bool state)
    : node_id_{ node_id }, state_{ state }, last_value_{}, update_time_{ time_helper::get_current_milliseconds() }
{
}

celeritas::red_dot_type celeritas::red_dots::get_node_id() const
{
    return node_id_;
}

void celeritas::red_dots::set_node_id(const red_dot_type nodeId)
{
    node_id_ = nodeId;
}

bool celeritas::red_dots::is_state() const
{
    return state_;
}

void celeritas::red_dots::set_state(const bool state)
{
    state_ = state;
}

int64_t celeritas::red_dots::get_last_value() const
{
    return last_value_;
}

void celeritas::red_dots::set_last_value(const int64_t lastValue)
{
    last_value_ = lastValue;
}

int64_t celeritas::red_dots::get_update_time() const
{
    return update_time_;
}

void celeritas::red_dots::set_update_time(const int64_t updateTime)
{
    update_time_ = updateTime;
}

std::string celeritas::red_dots::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ node_id_description }, static_cast<int32_t>(node_id_)));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ state_description }, state_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ last_value_description }, last_value_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ update_time_description }, update_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::red_dots celeritas::red_dots::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    red_dots red_dots{};
    red_dots.set_node_id(static_cast<red_dot_type>(parsed_view[node_id_description].get_int32().value));
    red_dots.set_state(parsed_view[state_description].get_bool());
    red_dots.set_last_value(parsed_view[last_value_description].type() == bsoncxx::type::k_int32 ? parsed_view[last_value_description].get_int32().value : parsed_view[last_value_description].get_int64().value);
    red_dots.set_last_value(parsed_view[update_time_description].type() == bsoncxx::type::k_int32 ? parsed_view[update_time_description].get_int32().value : parsed_view[update_time_description].get_int64().value);

    return red_dots;
}