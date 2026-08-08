// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#include "announcement_admin_list_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::announcement_admin_list_response::announcement_admin_list_response(const game_error_type code)
    : bass_type{ code }, total_{ 0 }, announcements_{}
{
}

celeritas::announcement_admin_list_response::announcement_admin_list_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, total_{ 0 }, announcements_{}
{
}

celeritas::announcement_admin_list_response::announcement_admin_list_response(const game_error_type code,
                                                                              std::string message,
                                                                              const int64_t total,
                                                                              container_type announcements)
    : bass_type{ code, std::move(message) }, total_{ total }, announcements_{ std::move(announcements) }
{
}

celeritas::announcement_admin_list_response::announcement_admin_list_response(bass_type http_response,
                                                                              const int64_t total,
                                                                              container_type announcements)
    : bass_type{ std::move(http_response) }, total_{ total }, announcements_{ std::move(announcements) }
{
}

celeritas::announcement_admin_list_response::announcement_admin_list_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, total_{ 0 }, announcements_{}
{
}

int64_t celeritas::announcement_admin_list_response::get_total() const noexcept
{
    return total_;
}

void celeritas::announcement_admin_list_response::set_total(const int64_t total)
{
    total_ = total;
}

const celeritas::announcement_admin_list_response::container_type& celeritas::announcement_admin_list_response::get_announcements() const noexcept
{
    return announcements_;
}

void celeritas::announcement_admin_list_response::set_announcements(container_type announcements)
{
    announcements_ = std::move(announcements);
}

std::string celeritas::announcement_admin_list_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::announcement_admin_list_response celeritas::announcement_admin_list_response::from_json_string(const std::string& json_string)
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

celeritas::announcement_admin_list_response celeritas::announcement_admin_list_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    const auto total = boost::json::value_to<int64_t>(object.at(total_description));
    auto announcements = boost::json::value_to<container_type>(object.at(announcements_description));

    return announcement_admin_list_response{ std::move(http_response), total, std::move(announcements) };
}

celeritas::announcement_admin_list_response celeritas::announcement_admin_list_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<announcement_admin_list_response>(value);
}

celeritas::announcement_admin_list_response celeritas::tag_invoke(announcement_admin_list_response_tag, const http_response::json_value& value)
{
    try
    {
        return announcement_admin_list_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, announcement_admin_list_response::json_value& value, const announcement_admin_list_response& announcement_admin_list_response)
{
    value = {
        { announcement_admin_list_response::code_description, enum_cast_underlying(announcement_admin_list_response.get_code()) },
        { announcement_admin_list_response::message_description, announcement_admin_list_response.get_message() },
        { announcement_admin_list_response::total_description, announcement_admin_list_response.get_total() },
        { announcement_admin_list_response::announcements_description, boost::json::value_from(announcement_admin_list_response.get_announcements()) }
    };
}
