#include "login_servers_response.h"
#include "common/celeritas_error.h"
#include "common/enum_cast.h"
#include "common/logger.h"

using namespace std::literals;

celeritas::login_servers_response::login_servers_response(const game_error_type code)
    : bass_type{ code }, login_server_info_{}
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, login_server_info_{}
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message, container_type login_server_info)
    : bass_type{ code, std::move(message) }, login_server_info_{ std::move(login_server_info) }
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message, login_server_info login_server_info)
    : bass_type{ code, std::move(message) }, login_server_info_{ std::move(login_server_info) }
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::login_servers_response::login_servers_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, login_server_info_{}
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(get_code()) << ",message:" << get_message();
}

celeritas::login_servers_response::login_servers_response(bass_type http_response, container_type login_server_info)
    : bass_type{ std::move(http_response) }, login_server_info_{ std::move(login_server_info) }
{
    LOG_CHANNEL(auth_channel, trace) << "login servers code:" << get_game_error_description(get_code()) << ",message:" << get_message();
}

celeritas::login_servers_response::container_type celeritas::login_servers_response::get_login_server_info() const
{
    return login_server_info_;
}

std::string celeritas::login_servers_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::login_servers_response celeritas::login_servers_response::from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<login_servers_response>(value);
}

celeritas::login_servers_response celeritas::login_servers_response::tag_invoke(const json_value& value)
{
    const auto& object = value.as_object();
    auto http_response = bass_type::tag_invoke(value);
    auto login_server_info = boost::json::value_to<container_type>(object.at(login_server_info_description));

    return login_servers_response{ std::move(http_response), std::move(login_server_info) };
}

celeritas::login_servers_response celeritas::tag_invoke(login_servers_response_tag, const login_servers_response::json_value& value)
{
    try
    {
        return login_servers_response::tag_invoke(value);
    }
    catch (const std::out_of_range& error)
    {
        throw celeritas_error{ "json deserialization failed: missing 'code' or 'message' key."s + error.what() };
    }
    catch (const boost::system::system_error& error)
    {
        throw celeritas_error{ "json deserialization failed: invalid value type for key."s + error.what() };
    }
}

void celeritas::tag_invoke(boost::json::value_from_tag, login_servers_response::json_value& value, const login_servers_response& login_servers_response)
{
    value = {
        { login_servers_response::code_description, enum_cast_underlying(login_servers_response.get_code()) },
        { login_servers_response::message_description, login_servers_response.get_message() },
        { login_servers_response::login_server_info_description, boost::json::value_from(login_servers_response.get_login_server_info()) }
    };
}