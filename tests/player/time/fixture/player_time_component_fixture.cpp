#include "player_time_component_fixture.h"
#include "database/pool/database_pool_manager.h"

#include <boost/asio/strand.hpp>

celeritas::player_time_component_fixture::player_time_component_fixture()
    : base_type{},
      mock_player_state_{ boost::asio::make_strand(io_context_) },
      mock_pool_{ std::make_shared<mock_database_pool>() },
      component_{ std::make_shared<player_time_component>(&mock_player_state_) },
      mock_player_component_{ std::make_shared<mock_player_component>(&mock_player_state_) }
{
    database_pool_manager::get_instance().set_mock_pool(mock_pool_);
    mock_player_state_.set_mock_player_component(mock_player_component_);
}

celeritas::player_time_component_fixture::~player_time_component_fixture()
{
    database_pool_manager::get_instance().set_mock_pool(nullptr);
}

void celeritas::player_time_component_fixture::run_io_context_two_times()
{
    // 由于存在定时器，避免卡死
    io_context_.restart();
    io_context_.run_one();
    io_context_.run_one();
}

celeritas::player_time_component_fixture::mock_database_pool_shared_ptr celeritas::player_time_component_fixture::get_mock_pool()
{
    return mock_pool_;
}

celeritas::player_time_component_fixture::player_time_component_shared_ptr celeritas::player_time_component_fixture::get_component()
{
    return component_;
}

celeritas::player_time_component_fixture::mock_player_component_shared_ptr celeritas::player_time_component_fixture::get_mock_player_component()
{
    return mock_player_component_;
}

celeritas::mock_player_state& celeritas::player_time_component_fixture::get_mock_player_state()
{
    return mock_player_state_;
}
