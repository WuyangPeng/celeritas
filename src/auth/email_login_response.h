#pragma once

#include "server/server_fwd.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class email_login_response
    {
    public:
        using class_type = email_login_response;

        email_login_response(game_error_type code, std::string message);

        email_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        [[nodiscard]] std::string to_json_string() const;

    private:
        game_error_type code_;
        std::string message_;
        std::string token_;
        int64_t expire_milliseconds_;
    };
}