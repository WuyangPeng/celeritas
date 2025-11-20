#pragma once

#include "server/server_fwd.h"

#include <string>

namespace celeritas
{
    class guest_login_response
    {
    public:
        using class_type = guest_login_response;

        guest_login_response(game_error_type code, std::string message);

        guest_login_response(game_error_type code, std::string message, std::string token);

        [[nodiscard]] std::string to_json_string() const;

    private:
        game_error_type code_;
        std::string message_;
        std::string token_;
    };
}