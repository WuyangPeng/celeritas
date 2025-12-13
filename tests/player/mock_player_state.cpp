#include "mock_player_state.h"
#include "config/database_type.h"

celeritas::mock_player_state::mock_player_state()
    : base_type{ user{ database_type::mysql, 123 }, nullptr, "test" }
{
}