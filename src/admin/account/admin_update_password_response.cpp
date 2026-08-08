// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_update_password_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::admin_update_password_response::admin_update_password_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::admin_update_password_response::admin_update_password_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::admin_update_password_response::admin_update_password_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

std::string celeritas::admin_update_password_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::admin_update_password_response celeritas::admin_update_password_response::from_json_string(const std::string& json_string)
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

celeritas::admin_update_password_response celeritas::admin_update_password_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);
    return admin_update_password_response{ std::move(http_response) };
}

celeritas::admin_update_password_response celeritas::admin_update_password_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<admin_update_password_response>(value);
}

celeritas::admin_update_password_response celeritas::tag_invoke(admin_update_password_response_tag, const http_response::json_value& value)
{
    try
    {
        return admin_update_password_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, admin_update_password_response::json_value& value, const admin_update_password_response& admin_update_password_response)
{
    value = {
        { admin_update_password_response::code_description, enum_cast_underlying(admin_update_password_response.get_code()) },
        { admin_update_password_response::message_description, admin_update_password_response.get_message() }
    };
}
