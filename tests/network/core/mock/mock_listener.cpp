#include "mock_listener.h"

celeritas::mock_listener::mock_listener(const any_io_executor& any_io_executor,
                                        network_message_callback_weak_ptr callback,
                                        std::string game_server_id,
                                        const server_network_type server_network_type)
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
    stop_called_ = true;
}

celeritas::listener::void_awaitable_type celeritas::mock_listener::accept_connections()
{
    accept_connections_called_ = true;
    co_return;
}

celeritas::listener::session_shared_ptr celeritas::mock_listener::get_session(int64_t id)
{
    return session_to_return_;
}

bool celeritas::mock_listener::write(const std::string& server_type,
                                     const std::string& instance_id,
                                     const header& header,
                                     const protobuf_message& request)
{
    write_called_ = true;
    write_server_type_ = server_type;
    write_instance_id_ = instance_id;
    write_header_ = header;
    write_request_ = &request;

    return true;
}

bool celeritas::mock_listener::stop_called() const
{
    return stop_called_;
}

bool celeritas::mock_listener::accept_connections_called() const
{
    return accept_connections_called_;
}

void celeritas::mock_listener::set_session_to_return(session_shared_ptr session)
{
    session_to_return_ = std::move(session);
}

bool celeritas::mock_listener::write_called() const
{
    return write_called_;
}

std::string celeritas::mock_listener::get_write_server_type() const
{
    return write_server_type_;
}

std::string celeritas::mock_listener::get_write_instance_id() const
{
    return write_instance_id_;
}

celeritas::header celeritas::mock_listener::get_write_header() const
{
    return write_header_;
}

const celeritas::listener::protobuf_message* celeritas::mock_listener::get_write_request() const
{
    return write_request_;
}
