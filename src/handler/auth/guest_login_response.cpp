#include "guest_login_response.h"
#include "common/celeritas_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

celeritas::guest_login_response::guest_login_response(const game_error_type code, std::string message)
    : code_{ code }, message_{ std::move(message) }, token_{}
{
}

celeritas::guest_login_response::guest_login_response(const game_error_type code, std::string message, std::string token)
    : code_{ code }, message_{ std::move(message) }, token_{ std::move(token) }
{
}

std::string celeritas::guest_login_response::to_json_string() const
{
    boost::property_tree::ptree tree{};
    tree.put("code", static_cast<int>(code_));
    tree.put("message", message_);
    tree.put("token", token_);

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