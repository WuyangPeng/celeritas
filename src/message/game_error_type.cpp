#include "game_error_type.h"

#include <map>

std::string celeritas::get_game_error_description(const game_error_type game_error)
{
    using game_error_container_type = std::map<game_error_type, std::string>;

    static game_error_container_type container{ { game_error_type::unknown, "unknown error." },
                                                { game_error_type::success, "success." },

                                                { game_error_type::invalid_parameter, "invalid parameter" },
                                                { game_error_type::timestamp_expired, "timestamp expired" },
                                                { game_error_type::sign_error, "sign error" },
                                                { game_error_type::sent_too_frequently, "sent too frequently" },
                                                { game_error_type::code_expired, "code expired" },
                                                { game_error_type::code_error, "code error" },
                                                { game_error_type::token_error, "token error" },
                                                { game_error_type::password_error, "password error" },

                                                { game_error_type::account_error, "account error" },
                                                { game_error_type::no_guest_account, "no guest account" },
                                                { game_error_type::account_bound, "account bound" },

                                                { game_error_type::redis_error, "redis error" },
                                                { game_error_type::mysql_error, "mysql error" } };

    if (const auto iter = container.find(game_error);
        iter != container.cend())
    {
        return iter->second;
    }

    return "error undefined.";
}