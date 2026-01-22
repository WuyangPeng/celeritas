#include "mock_session.h"
#include "config/basic/server_network_type.h"

int64_t celeritas::mock_session::get_session_id() const noexcept
{
    return 0;
}

void celeritas::mock_session::write(const header& header, const protobuf_message_type& response)
{
}

celeritas::session::void_awaitable_type celeritas::mock_session::write_immediately(const std::string& response)
{
    co_return;
}

void celeritas::mock_session::remove_session()
{
}

celeritas::server_network_type celeritas::mock_session::get_server_network_type() const
{
    return server_network_type::tcp;
}

void celeritas::mock_session::set_instance_id(const std::string& instance_id)
{
}

std::string celeritas::mock_session::get_instance_id() const
{
    return "";
}