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

                                                { game_error_type::redis_error, "redis error" } };

    if (const auto iter = container.find(game_error);
        iter != container.cend())
    {
        return iter->second;
    }

    return "error undefined.";
}