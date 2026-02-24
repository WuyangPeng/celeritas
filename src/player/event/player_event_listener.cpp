#include "player_event_listener.h"

celeritas::player_event_priority celeritas::player_event_listener::get_priority() const noexcept
{
    return player_event_priority::normal;
}
