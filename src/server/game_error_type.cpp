#include "game_error_type.h"

#include <map>

std::string celeritas::get_game_error_description(const game_error_type game_error)
{
    using game_error_container_type = std::map<game_error_type, std::string>;

    static game_error_container_type container{ { game_error_type::unknown, "unknown error." },
                                                { game_error_type::success, "success." }, };

    if (const auto iter = container.find(game_error);
        iter != container.cend())
    {
        return iter->second;
    }

    return "error undefined.";
}