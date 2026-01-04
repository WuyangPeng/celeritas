#include "http_response.h"
#include "common/core/celeritas_error.h"

using namespace std::literals;

celeritas::http_response::http_response(const game_error_type code)
    : code_{ code }, message_{ get_game_error_description(code) }
{
}

celeritas::http_response::http_response(const game_error_type code, std::string message)
    : code_{ code }, message_{ std::move(message) }
{
}

celeritas::game_error_type celeritas::http_response::get_code() const
{
    return code_;
}

void celeritas::http_response::set_code(const game_error_type code)
{
    code_ = code;
}

std::string celeritas::http_response::get_message() const
{
    return message_;
}

void celeritas::http_response::set_message(const std::string& message)
{
    message_ = message;
}

std::string celeritas::http_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::http_response celeritas::http_response::from_json_string(const std::string& json_string)
{
    try
    {
        return do_from_json_string(json_string);
    }
    catch (const std::exception& e)
    {
        throw celeritas_error{ "json deserialization failed: "s + e.what() };
    }
}

celeritas::http_response celeritas::http_response::tag_invoke(const json_value& value)
{
    const auto& object = value.as_object();
    const auto code = static_cast<game_error_type>(boost::json::value_to<int>(object.at(code_description)));
    auto message = boost::json::value_to<std::string>(object.at(message_description));

    return http_response{ code, std::move(message) };
}

celeritas::http_response celeritas::http_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<http_response>(value);
}

celeritas::http_response celeritas::tag_invoke(http_response_tag, const http_response::json_value& value)
{
    try
    {
        return http_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, http_response::json_value& value, const http_response& http_response)
{
    value = {
        { http_response::code_description, static_cast<int>(http_response.get_code()) },
        { http_response::message_description, http_response.get_message() }
    };
}