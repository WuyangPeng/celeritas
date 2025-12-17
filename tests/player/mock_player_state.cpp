#include "mock_player_state.h"
#include "config/database_type.h"

celeritas::mock_player_state::mock_player_state(io_context_type& io_context)
    : base_type{ user{ database_type::mysql, 123 }, nullptr, io_context, "test", proto::service::service_login_request::default_instance() }
{
}