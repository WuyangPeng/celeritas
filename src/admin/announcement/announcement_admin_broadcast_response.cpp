// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_broadcast_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::announcement_admin_broadcast_response::announcement_admin_broadcast_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::announcement_admin_broadcast_response::announcement_admin_broadcast_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::announcement_admin_broadcast_response::announcement_admin_broadcast_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

std::string celeritas::announcement_admin_broadcast_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::announcement_admin_broadcast_response celeritas::announcement_admin_broadcast_response::from_json_string(const std::string& json_string)
{
    try
    {
        return do_from_json_string(json_string);
    }
    catch (const std::exception& e)
    {
        throw celeritas_error{ "json deserialization failed: {}", e.what() };
    }
}

celeritas::announcement_admin_broadcast_response celeritas::announcement_admin_broadcast_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);
    return announcement_admin_broadcast_response{ std::move(http_response) };
}

celeritas::announcement_admin_broadcast_response celeritas::announcement_admin_broadcast_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<announcement_admin_broadcast_response>(value);
}

celeritas::announcement_admin_broadcast_response celeritas::tag_invoke(announcement_admin_broadcast_response_tag, const http_response::json_value& value)
{
    try
    {
        return announcement_admin_broadcast_response::tag_invoke(value);
    }
    catch (const std::out_of_range& error)
    {
        throw celeritas_error{ "json deserialization failed: missing 'code' or 'message' key. {}", error.what() };
    }
    catch (const boost::system::system_error& error)
    {
        throw celeritas_error{ "json deserialization failed: invalid value type for key. {}", error.what() };
    }
}

void celeritas::tag_invoke(boost::json::value_from_tag, announcement_admin_broadcast_response::json_value& value, const announcement_admin_broadcast_response& announcement_admin_broadcast_response)
{
    value = {
        { announcement_admin_broadcast_response::code_description, enum_cast_underlying(announcement_admin_broadcast_response.get_code()) },
        { announcement_admin_broadcast_response::message_description, announcement_admin_broadcast_response.get_message() }
    };
}
