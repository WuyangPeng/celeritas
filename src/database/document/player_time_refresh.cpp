#include "player_time_refresh.h"
#include "player/time/time_refresh_type.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::player_time_refresh::player_time_refresh() noexcept
    : time_refresh_{ time_refresh_type::null }, parameter_{}, last_refresh_time_{}
{
}

celeritas::time_refresh_type celeritas::player_time_refresh::get_time_refresh() const
{
    return time_refresh_;
}

int celeritas::player_time_refresh::get_parameter() const
{
    return parameter_;
}

int64_t celeritas::player_time_refresh::get_last_refresh_time() const
{
    return last_refresh_time_;
}

void celeritas::player_time_refresh::set_time_refresh(const time_refresh_type timeRefresh)
{
    time_refresh_ = timeRefresh;
}

void celeritas::player_time_refresh::set_parameter(const int parameter)
{
    parameter_ = parameter;
}

void celeritas::player_time_refresh::set_last_refresh_time(const int64_t lastRefreshTime)
{
    last_refresh_time_ = lastRefreshTime;
}

std::string celeritas::player_time_refresh::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ time_refresh_description }, static_cast<int>(time_refresh_)));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ parameter_description }, parameter_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ last_refresh_time_description }, last_refresh_time_));

    return bsoncxx::to_json(builder.view());
}

celeritas::player_time_refresh celeritas::player_time_refresh::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    player_time_refresh player_time_refresh{};
    player_time_refresh.set_time_refresh(static_cast<time_refresh_type>(parsed_view[time_refresh_description].get_int32().value));
    player_time_refresh.set_parameter(parsed_view[parameter_description].get_int32());
    player_time_refresh.set_last_refresh_time(parsed_view[last_refresh_time_description].get_int64());

    return player_time_refresh;
}