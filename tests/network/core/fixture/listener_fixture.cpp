#include "listener_fixture.h"
#include "network/core/mock/mock_listener.h"

celeritas::listener_fixture::listener_fixture()
    : io_context_{},
      callback_{},
      game_server_id_{ "test_server" },
      server_type_{ server_network_type::tcp },
      listener_{ std::make_shared<mock_listener>(io_context_.get_executor(), callback_, game_server_id_, server_type_) }
{
}

celeritas::listener_fixture::io_context_type& celeritas::listener_fixture::get_io_context()
{
    return io_context_;
}

const std::string& celeritas::listener_fixture::get_game_server_id() const
{
    return game_server_id_;
}

celeritas::server_network_type celeritas::listener_fixture::get_server_type() const
{
    return server_type_;
}

celeritas::listener_fixture::mock_listener_shared_ptr celeritas::listener_fixture::get_listener()
{
    return listener_;
}

