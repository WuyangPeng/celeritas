#pragma once

#include "login_server_info.h"
#include "message/http_response.h"

namespace celeritas
{
    class login_servers_response final : public http_response
    {
    public:
        using class_type = login_servers_response;
        using bass_type = http_response;
        using container = std::vector<login_server_info>;

        login_servers_response() noexcept = default;

        explicit login_servers_response(game_error_type code);

        login_servers_response(game_error_type code, std::string message);

        login_servers_response(game_error_type code, std::string message, container login_server_info);

        explicit login_servers_response(bass_type http_response);

        login_servers_response(bass_type http_response, container login_server_info);

    private:
        container login_server_info_;
    };
}

