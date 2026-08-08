// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_create_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::announcement_admin_create_response::announcement_admin_create_response(const game_error_type code)
    : bass_type{ code }, id_{ 0 }, title_{}, status_{}
{
}

celeritas::announcement_admin_create_response::announcement_admin_create_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, id_{ 0 }, title_{}, status_{}
{
}

celeritas::announcement_admin_create_response::announcement_admin_create_response(const game_error_type code,
                                                                                  std::string message,
                                                                                  const int64_t id,
                                                                                  std::string title,
                                                                                  const announcement_status status)
    : bass_type{ code, std::move(message) }, id_{ id }, title_{ std::move(title) }, status_{ status }
{
}

celeritas::announcement_admin_create_response::announcement_admin_create_response(bass_type http_response,
                                                                                  const int64_t id,
                                                                                  std::string title,
                                                                                  const announcement_status status)
    : bass_type{ std::move(http_response) }, id_{ id }, title_{ std::move(title) }, status_{ status }
{
}

celeritas::announcement_admin_create_response::announcement_admin_create_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, id_{ 0 }, title_{}, status_{}
{
}

int64_t celeritas::announcement_admin_create_response::get_id() const noexcept
{
    return id_;
}

void celeritas::announcement_admin_create_response::set_id(const int64_t id) noexcept
{
    id_ = id;
}

const std::string& celeritas::announcement_admin_create_response::get_title() const noexcept
{
    return title_;
}

void celeritas::announcement_admin_create_response::set_title(std::string title)
{
    title_ = std::move(title);
}

celeritas::announcement_status celeritas::announcement_admin_create_response::get_status() const noexcept
{
    return status_;
}

void celeritas::announcement_admin_create_response::set_status(const announcement_status status)
{
    status_ = status;
}

std::string celeritas::announcement_admin_create_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::announcement_admin_create_response celeritas::announcement_admin_create_response::from_json_string(const std::string& json_string)
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

celeritas::announcement_admin_create_response celeritas::announcement_admin_create_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    const auto id = std::stoll(boost::json::value_to<std::string>(object.at(id_description)));
    auto title = boost::json::value_to<std::string>(object.at(title_description));
    const auto status = underlying_cast_enum<announcement_status>(boost::json::value_to<int32_t>(object.at(status_description)));

    return announcement_admin_create_response{ std::move(http_response), id, std::move(title), status };
}

celeritas::announcement_admin_create_response celeritas::announcement_admin_create_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<announcement_admin_create_response>(value);
}

celeritas::announcement_admin_create_response celeritas::tag_invoke(announcement_admin_create_response_tag, const http_response::json_value& value)
{
    try
    {
        return announcement_admin_create_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, announcement_admin_create_response::json_value& value, const announcement_admin_create_response& announcement_admin_create_response)
{
    value = {
        { announcement_admin_create_response::code_description, enum_cast_underlying(announcement_admin_create_response.get_code()) },
        { announcement_admin_create_response::message_description, announcement_admin_create_response.get_message() },
        { announcement_admin_create_response::id_description, std::to_string(announcement_admin_create_response.get_id()) },
        { announcement_admin_create_response::title_description, announcement_admin_create_response.get_title() },
        { announcement_admin_create_response::status_description, enum_cast_underlying(announcement_admin_create_response.get_status()) }
    };
}
