// 创建时间：2026-07-26
// 修改时间：2026-07-26
// 审核时间：2026-07-26

#include "admin_create_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::admin_create_response::admin_create_response(const game_error_type code)
    : bass_type{ code }, account_id_{ 0 }, username_{}
{
}

celeritas::admin_create_response::admin_create_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, account_id_{ 0 }, username_{}
{
}

celeritas::admin_create_response::admin_create_response(const game_error_type code,
                                                        std::string message,
                                                        const int64_t account_id,
                                                        std::string username)
    : bass_type{ code, std::move(message) }, account_id_{ account_id }, username_{ std::move(username) }
{
}

celeritas::admin_create_response::admin_create_response(bass_type http_response,
                                                        const int64_t account_id,
                                                        std::string username)
    : bass_type{ std::move(http_response) }, account_id_{ account_id }, username_{ std::move(username) }
{
}

celeritas::admin_create_response::admin_create_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, account_id_{ 0 }, username_{}
{
}

int64_t celeritas::admin_create_response::get_account_id() const
{
    return account_id_;
}

void celeritas::admin_create_response::set_account_id(const int64_t account_id)
{
    account_id_ = account_id;
}

std::string celeritas::admin_create_response::get_username() const
{
    return username_;
}

void celeritas::admin_create_response::set_username(const std::string& username)
{
    username_ = username;
}

std::string celeritas::admin_create_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::admin_create_response celeritas::admin_create_response::from_json_string(const std::string& json_string)
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

celeritas::admin_create_response celeritas::admin_create_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    const auto account_id = boost::json::value_to<int64_t>(object.at(account_id_description));
    auto username = boost::json::value_to<std::string>(object.at(username_description));

    return admin_create_response{ std::move(http_response), account_id, std::move(username) };
}

celeritas::admin_create_response celeritas::admin_create_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<admin_create_response>(value);
}

celeritas::admin_create_response celeritas::tag_invoke(admin_create_response_tag, const http_response::json_value& value)
{
    try
    {
        return admin_create_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, admin_create_response::json_value& value, const admin_create_response& admin_create_response)
{
    value = {
        { admin_create_response::code_description, enum_cast_underlying(admin_create_response.get_code()) },
        { admin_create_response::message_description, admin_create_response.get_message() },
        { admin_create_response::account_id_description, admin_create_response.get_account_id() },
        { admin_create_response::username_description, admin_create_response.get_username() }
    };
}
