#pragma once

#include "player/component/player_state.h"

namespace celeritas
{
    class mock_player_state final : public player_state
    {
    public:
        using class_type = mock_player_state;
        using base_type = player_state;

        explicit mock_player_state(io_context_type& io_context);
    };
}