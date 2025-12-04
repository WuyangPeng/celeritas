#include "login_servers_response.h"

celeritas::login_servers_response::login_servers_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::login_servers_response::login_servers_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
}