#include "login_servers_response.h"

celeritas::login_servers_response::login_servers_response(const game_error_type code)
    : bass_type{ code }, login_server_info_{}
{
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, login_server_info_{}
{
}

celeritas::login_servers_response::login_servers_response(const game_error_type code, std::string message, container login_server_info)
    : bass_type{ code, std::move(message) }, login_server_info_{ std::move(login_server_info) }
{
}

celeritas::login_servers_response::login_servers_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, login_server_info_{}
{
}

celeritas::login_servers_response::login_servers_response(bass_type http_response, container login_server_info)
    : bass_type{ std::move(http_response) }, login_server_info_{ std::move(login_server_info) }
{
}