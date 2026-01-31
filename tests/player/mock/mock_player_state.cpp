#include "mock_player_state.h"
#include "config/basic/database_type.h"

celeritas::mock_player_state::mock_player_state(const any_io_executor& any_io_executor)
    : base_type{ user{ database_type::mysql, 123 }, nullptr, any_io_executor, "test", proto::service::service_login_request::default_instance(), false }
{
}