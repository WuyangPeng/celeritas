#pragma once

#include "message/game_error_type.h"
#include "common/core_utilities/celeritas_error.h"

namespace celeritas
{
    class celeritas_game_error final : public celeritas_error
    {
    public:
        using class_type = celeritas_game_error;
        using base_type = celeritas_error;

        explicit celeritas_game_error(game_error_type game_error_type);

        celeritas_game_error(game_error_type game_error_type, const std::string& error);
    };
}