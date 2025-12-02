#include "order_create_http_response.h"
#include "common/celeritas_error.h"

using namespace std::literals;

celeritas::order_create_http_response::order_create_http_response(const game_error_type code)
    : bass_type{ code }, order_id_{}, platform_{}, payment_params_json_{}, amount_{ 0 }
{
}

celeritas::order_create_http_response::order_create_http_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, order_id_{}, platform_{}, payment_params_json_{}, amount_{ 0 }
{
}

celeritas::order_create_http_response::order_create_http_response(const game_error_type code, std::string message, std::string order_id, const payment_platform_type platform, std::string payment_params_json, int amount)
    : bass_type{ code, std::move(message) }, order_id_{ std::move(order_id) }, platform_{ platform }, payment_params_json_{ std::move(payment_params_json) }, amount_{ amount }
{
}

celeritas::order_create_http_response::order_create_http_response(bass_type http_response, std::string order_id, const payment_platform_type platform, std::string payment_params_json, int amount)
    : bass_type{ std::move(http_response) }, order_id_{ std::move(order_id) }, platform_{ platform }, payment_params_json_{ std::move(payment_params_json) }, amount_{ amount }
{
}

celeritas::order_create_http_response::order_create_http_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, order_id_{}, platform_{}, payment_params_json_{}, amount_{ 0 }
{
}

std::string celeritas::order_create_http_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::order_create_http_response celeritas::order_create_http_response::from_json_string(const std::string& json_string)
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

celeritas::order_create_http_response celeritas::order_create_http_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    auto order_id = boost::json::value_to<std::string>(object.at(order_id_description));
    const auto platform = static_cast<payment_platform_type>(boost::json::value_to<int>(object.at(platform_description)));
    auto payment_params_json = boost::json::value_to<std::string>(object.at(payment_params_json_description));
    const auto amount = boost::json::value_to<int>(object.at(amount_description));

    return order_create_http_response{ std::move(http_response), std::move(order_id), platform, std::move(payment_params_json), amount };
}

celeritas::order_create_http_response celeritas::order_create_http_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<order_create_http_response>(value);
}

std::string celeritas::order_create_http_response::get_order_id() const
{
    return order_id_;
}

celeritas::payment_platform_type celeritas::order_create_http_response::get_platform() const
{
    return platform_;
}

std::string celeritas::order_create_http_response::get_payment_params_json() const
{
    return payment_params_json_;
}

int celeritas::order_create_http_response::get_amount() const
{
    return amount_;
}

celeritas::order_create_http_response celeritas::tag_invoke(order_create_http_response_tag, const http_response::json_value& value)
{
    try
    {
        return order_create_http_response::tag_invoke(value);
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

void celeritas::tag_invoke(boost::json::value_from_tag, order_create_http_response::json_value& value, const order_create_http_response& order_create_http_response)
{
    value = {
        { order_create_http_response::order_id_description, order_create_http_response.get_order_id() },
        { order_create_http_response::platform_description, static_cast<int>(order_create_http_response.get_platform()) },
        { order_create_http_response::payment_params_json_description, order_create_http_response.get_payment_params_json() },
        { order_create_http_response::amount_description, order_create_http_response.get_amount() }
    };
}