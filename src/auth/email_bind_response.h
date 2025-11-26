#pragma once

#include "server/server_fwd.h"

#include <string>

namespace celeritas
{
    class email_bind_response
    {
    public:
        using class_type = email_bind_response;

        email_bind_response(game_error_type code, std::string message);

        [[nodiscard]] std::string to_json_string() const;

    private:
        game_error_type code_;
        std::string message_;
    };
}