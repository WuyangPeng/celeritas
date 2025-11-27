#pragma once

#include "message/message_fwd.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class phone_login_response
    {
    public:
        using class_type = phone_login_response;

        phone_login_response(game_error_type code, std::string message);

        phone_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        [[nodiscard]] std::string to_json_string() const;

    private:
        game_error_type code_;
        std::string message_;
        std::string token_;
        int64_t expire_milliseconds_;
    };
}