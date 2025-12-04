#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class login_servers_response final : public http_response
    {
    public:
        using class_type = login_servers_response;
        using bass_type = http_response;

        login_servers_response() noexcept = default;

        explicit login_servers_response(game_error_type code);

        login_servers_response(game_error_type code, std::string message);

        explicit login_servers_response(bass_type token_http_response);
    };
}

