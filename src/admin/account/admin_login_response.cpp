// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_login_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::admin_login_response::admin_login_response(const game_error_type code)
    : bass_type{ code }, token_{}, account_id_{}, username_{}, role_{ 0 }, avatar_{}
{
}

celeritas::admin_login_response::admin_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, token_{}, account_id_{}, username_{}, role_{ 0 }, avatar_{}
{
}

celeritas::admin_login_response::admin_login_response(const game_error_type code,
                                                      std::string message,
                                                      std::string token,
                                                      std::string account_id,
                                                      std::string username,
                                                      const int32_t role,
                                                      std::string avatar)
    : bass_type{ code, std::move(message) },
      token_{ std::move(token) },
      account_id_{ std::move(account_id) },
      username_{ std::move(username) },
      role_{ role },
      avatar_{ std::move(avatar) }
{
}

celeritas::admin_login_response::admin_login_response(bass_type http_response,
                                                      std::string token,
                                                      std::string account_id,
                                                      std::string username,
                                                      const int32_t role,
                                                      std::string avatar)
    : bass_type{ std::move(http_response) },
      token_{ std::move(token) },
      account_id_{ std::move(account_id) },
      username_{ std::move(username) },
      role_{ role },
      avatar_{ std::move(avatar) }
{
}

celeritas::admin_login_response::admin_login_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, token_{}, account_id_{}, username_{}, role_{ 0 }, avatar_{}
{
}

const std::string& celeritas::admin_login_response::get_token() const noexcept
{
    return token_;
}

void celeritas::admin_login_response::set_token(const std::string& token)
{
    token_ = token;
}

const std::string& celeritas::admin_login_response::get_account_id() const noexcept
{
    return account_id_;
}

void celeritas::admin_login_response::set_account_id(const std::string& account_id)
{
    account_id_ = account_id;
}

const std::string& celeritas::admin_login_response::get_username() const noexcept
{
    return username_;
}

void celeritas::admin_login_response::set_username(const std::string& username)
{
    username_ = username;
}

int32_t celeritas::admin_login_response::get_role() const noexcept
{
    return role_;
}

void celeritas::admin_login_response::set_role(const int32_t role)
{
    role_ = role;
}

const std::string& celeritas::admin_login_response::get_avatar() const noexcept
{
    return avatar_;
}

void celeritas::admin_login_response::set_avatar(const std::string& avatar)
{
    avatar_ = avatar;
}

std::string celeritas::admin_login_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::admin_login_response celeritas::admin_login_response::from_json_string(const std::string& json_string)
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

celeritas::admin_login_response celeritas::admin_login_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    auto token = boost::json::value_to<std::string>(object.at(token_description));
    auto account_id = boost::json::value_to<std::string>(object.at(account_id_description));
    auto username = boost::json::value_to<std::string>(object.at(username_description));
    const auto role = boost::json::value_to<int32_t>(object.at(role_description));
    auto avatar = boost::json::value_to<std::string>(object.at(avatar_description));

    return admin_login_response{ std::move(http_response), std::move(token), std::move(account_id), std::move(username), role, std::move(avatar) };
}

celeritas::admin_login_response celeritas::admin_login_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<admin_login_response>(value);
}

celeritas::admin_login_response celeritas::tag_invoke(admin_login_response_tag, const http_response::json_value& value)
{
    try
    {
        return admin_login_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, admin_login_response::json_value& value, const admin_login_response& admin_login_response)
{
    value = {
        { admin_login_response::code_description, enum_cast_underlying(admin_login_response.get_code()) },
        { admin_login_response::message_description, admin_login_response.get_message() },
        { admin_login_response::token_description, admin_login_response.get_token() },
        { admin_login_response::account_id_description, admin_login_response.get_account_id() },
        { admin_login_response::username_description, admin_login_response.get_username() },
        { admin_login_response::role_description, admin_login_response.get_role() },
        { admin_login_response::avatar_description, admin_login_response.get_avatar() }
    };
}
