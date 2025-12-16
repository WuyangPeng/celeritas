#include "player_time_refresh.h"
#include "common/time_helper.h"
#include "player/time/time_refresh_type.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::player_time_refresh::player_time_refresh() noexcept
    : time_refresh_type_{}, parameter_{}, component_{}, last_refresh_time_{}
{
}

celeritas::player_time_refresh::player_time_refresh(const time_refresh_type time_refresh_type, const int64_t parameter, const player_component_type player_component_type)
    : time_refresh_type_{ time_refresh_type }, parameter_{ parameter }, component_{ player_component_type }, last_refresh_time_{ time_helper::get_current_milliseconds() }
{
}

celeritas::time_refresh_type celeritas::player_time_refresh::get_time_refresh_type() const
{
    return time_refresh_type_;
}

void celeritas::player_time_refresh::set_time_refresh_type(const time_refresh_type timeRefreshType)
{
    time_refresh_type_ = timeRefreshType;
}

int64_t celeritas::player_time_refresh::get_parameter() const
{
    return parameter_;
}

void celeritas::player_time_refresh::set_parameter(const int64_t parameter)
{
    parameter_ = parameter;
}

celeritas::player_time_refresh::component_container celeritas::player_time_refresh::get_component() const
{
    return component_;
}

void celeritas::player_time_refresh::set_component(const component_container& component)
{
    component_ = component;
}

void celeritas::player_time_refresh::add_component(player_component_type component)
{
    component_.emplace_back(component);
}

void celeritas::player_time_refresh::remove_component(const player_component_type playerComponent)
{
    std::erase(component_, playerComponent);
}

int64_t celeritas::player_time_refresh::get_last_refresh_time() const
{
    return last_refresh_time_;
}

void celeritas::player_time_refresh::set_last_refresh_time(const int64_t last_refresh_time)
{
    last_refresh_time_ = last_refresh_time;
}

bool celeritas::player_time_refresh::is_can_refresh() const
{
    return get_next_refresh_time() >= time_helper::get_current_milliseconds();
}

int64_t celeritas::player_time_refresh::get_next_refresh_time() const
{
    switch (time_refresh_type_)
    {
        case time_refresh_type::daily:
        {
            return time_helper::get_current_milliseconds() >= time_helper::get_end_of_day_milliseconds_with_offset(parameter_);
        }
        case time_refresh_type::weekly:
        {
            return time_helper::get_current_milliseconds() >= time_helper::get_end_of_week_milliseconds_with_offset(parameter_);
        }
        case time_refresh_type::monthly:
        {
            return time_helper::get_current_milliseconds() >= time_helper::get_end_of_month_milliseconds_with_offset(parameter_);
        }
        case time_refresh_type::interval_duration:
        {
            return time_helper::get_current_milliseconds() >= last_refresh_time_ + parameter_;
        }
        case time_refresh_type::absolute_point:
        {
            return time_helper::get_current_milliseconds() >= parameter_;
        }
        default:
        {
            return 0;
        }
    }
}

std::string celeritas::player_time_refresh::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ time_refresh_type_description }, static_cast<int32_t>(time_refresh_type_)));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ parameter_description }, parameter_));
    bsoncxx::builder::basic::array basic{};
    for (const auto& element : component_)
    {
        basic.append(static_cast<int32_t>(element));
    }
    builder.append(bsoncxx::builder::basic::kvp(std::string{ component_description }, basic));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ last_refresh_time_description }, last_refresh_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::player_time_refresh celeritas::player_time_refresh::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    player_time_refresh player_time_refresh{};
    player_time_refresh.set_time_refresh_type(static_cast<time_refresh_type>(parsed_view[time_refresh_type_description].get_int32().value));
    player_time_refresh.set_parameter(parsed_view[parameter_description].get_int64());
    for (const auto& element : parsed_view[parameter_description].get_array().value)
    {
        player_time_refresh.add_component(static_cast<player_component_type>(element.get_int32().value));
    }
    player_time_refresh.set_last_refresh_time(parsed_view[last_refresh_time_description].get_int64());

    return player_time_refresh;
}