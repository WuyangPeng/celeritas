#include "player_mail_component.h"

celeritas::player_mail_component::player_mail_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}