#include "token_http_response.h"
#include "../../common/core_utilities/celeritas_error.h"

using namespace std::literals;

celeritas::token_http_response::token_http_response(const game_error_type code)
    : bass_type{ code }, token_{}, expire_milliseconds_{}
{
}

celeritas::token_http_response::token_http_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, token_{}, expire_milliseconds_{}
{
}

celeritas::token_http_response::token_http_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message) }, token_{ std::move(token) }, expire_milliseconds_{ expire_milliseconds }
{
}

celeritas::token_http_response::token_http_response(bass_type http_response, std::string token, const int64_t expire_milliseconds)
    : bass_type{ std::move(http_response) }, token_{ std::move(token) }, expire_milliseconds_{ expire_milliseconds }
{
}

celeritas::token_http_response::token_http_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, token_{}, expire_milliseconds_{}
{
}

std::string celeritas::token_http_response::get_token() const
{
    return token_;
}

void celeritas::token_http_response::set_token(const std::string& token)
{
    token_ = token;
}

int64_t celeritas::token_http_response::get_expire_milliseconds() const
{
    return expire_milliseconds_;
}

void celeritas::token_http_response::set_expire_milliseconds(const int64_t expireMilliseconds)
{
    expire_milliseconds_ = expireMilliseconds;
}

std::string celeritas::token_http_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::token_http_response celeritas::token_http_response::from_json_string(const std::string& json_string)
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

celeritas::token_http_response celeritas::token_http_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    auto token = boost::json::value_to<std::string>(object.at(token_description));
    const auto expire_milliseconds = boost::json::value_to<int64_t>(object.at(expire_milliseconds_description));

    return token_http_response{ std::move(http_response), std::move(token), expire_milliseconds };
}

celeritas::token_http_response celeritas::token_http_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<token_http_response>(value);
}

celeritas::token_http_response celeritas::tag_invoke(token_http_response_tag, const http_response::json_value& value)
{
    try
    {
        return token_http_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, token_http_response::json_value& value, const token_http_response& token_http_response)
{
    value = {
        { token_http_response::code_description, static_cast<int>(token_http_response.get_code()) },
        { token_http_response::message_description, token_http_response.get_message() },
        { token_http_response::token_description, token_http_response.get_token() },
        { token_http_response::expire_milliseconds_description, token_http_response.get_expire_milliseconds() }
    };
}