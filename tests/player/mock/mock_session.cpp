#include "mock_session.h"
#include "common/core/celeritas_error.h"
#include "config/basic/server_network_type.h"

celeritas::mock_session::mock_session(const any_io_executor& any_io_executor)
    : any_io_executor_{ any_io_executor }
{
}

void celeritas::mock_session::stop()
{
}

void celeritas::mock_session::write(const header& header, const protobuf_message_type& response)
{
    last_header_ = header;
}

void celeritas::mock_session::write(const std::string& response)
{
}

celeritas::session::void_awaitable_type celeritas::mock_session::write_immediately(const std::string& response)
{
    co_return;
}

int64_t celeritas::mock_session::get_session_id() const noexcept
{
    return session_id_;
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
    instance_id_ = instance_id;
}

std::string celeritas::mock_session::get_instance_id() const
{
    return instance_id_;
}

celeritas::game_error_type celeritas::mock_session::get_code() const
{
    return last_header_.get_code();
}

celeritas::session::any_io_executor celeritas::mock_session::get_any_io_executor()
{
    return any_io_executor_;
}