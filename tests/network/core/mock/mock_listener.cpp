#include "mock_listener.h"

celeritas::mock_listener::mock_listener(const any_io_executor& any_io_executor, network_message_callback_weak_ptr callback, std::string game_server_id, server_network_type server_network_type)
    : base_type{ any_io_executor, std::move(callback), std::move(game_server_id), server_network_type }
{
}

std::string celeritas::mock_listener::test_get_game_server_id() const
{
    return get_game_server_id();
}

celeritas::listener::network_message_callback_weak_ptr celeritas::mock_listener::test_get_network_message_callback()
{
    return get_network_message_callback();
}

void celeritas::mock_listener::stop()
{
}

celeritas::listener::void_awaitable_type celeritas::mock_listener::accept_connections()
{
    co_return;
}

celeritas::listener::session_shared_ptr celeritas::mock_listener::get_session(int64_t id)
{
    return nullptr;
}

bool celeritas::mock_listener::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    return false;
}