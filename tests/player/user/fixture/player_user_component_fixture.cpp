#include "player_user_component_fixture.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_manager.h"

celeritas::player_user_component_fixture::player_user_component_fixture()
    : base_type{},
      mock_pool_{ std::make_shared<mock_database_pool>() },
      test_user_{ database_type::mysql, 12345 },
      mock_player_state_{ boost::asio::make_strand(io_context_) }
{
    database_pool_manager::get_instance().set_mock_pool(mock_pool_);
}

celeritas::player_user_component_fixture::~player_user_component_fixture()
{
    database_pool_manager::get_instance().set_mock_pool(nullptr);
}

celeritas::player_user_component_fixture::mock_database_pool_shared_ptr celeritas::player_user_component_fixture::get_mock_pool()
{
    return mock_pool_;
}

celeritas::user& celeritas::player_user_component_fixture::get_test_user()
{
    return test_user_;
}

celeritas::mock_player_state& celeritas::player_user_component_fixture::get_mock_player_state()
{
    return mock_player_state_;
}
