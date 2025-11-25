#include "phone_login_response.h"
#include "common/celeritas_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

celeritas::phone_login_response::phone_login_response(const game_error_type code, std::string message)
    : code_{ code }, message_{ std::move(message) }, token_{}, expire_milliseconds_{}
{
}

celeritas::phone_login_response::phone_login_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : code_{ code }, message_{ std::move(message) }, token_{ std::move(token) }, expire_milliseconds_{ expire_milliseconds }
{
}

std::string celeritas::phone_login_response::to_json_string() const
{
    boost::property_tree::ptree tree{};
    tree.put("code", static_cast<int>(code_));
    tree.put("message", message_);
    tree.put("token", token_);
    tree.put("expire_milliseconds", expire_milliseconds_);

    try
    {
        std::stringstream string_stream{};
        boost::property_tree::write_json(string_stream, tree, false);
        return string_stream.str();
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
        throw celeritas_error("json serialization failed: " + std::string(e.what()));
    }
}