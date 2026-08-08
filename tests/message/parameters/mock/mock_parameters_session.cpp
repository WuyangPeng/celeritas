#include "mock_parameters_session.h"
#include "common/core/celeritas_error.h"
#include "config/basic/server_network_type.h"

#include <boost/asio/io_context.hpp>

celeritas::mock_parameters_session::mock_parameters_session()
    : executor_{}
{
}

celeritas::mock_parameters_session::mock_parameters_session(io_context_type& io_context)
    : executor_{ io_context.get_executor() }
{
}

void celeritas::mock_parameters_session::stop()
{
}

void celeritas::mock_parameters_session::write(const header& header, const protobuf_message_type& response)
{
    ++write_with_header_count;
    last_header = header;
}

celeritas::session::void_awaitable_type celeritas::mock_parameters_session::write_immediately(const std::string& response)
{
    write_immediately_call_ = true;
    co_return;
}

int64_t celeritas::mock_parameters_session::get_session_id() const noexcept
{
    return 125;
}

void celeritas::mock_parameters_session::remove_session()
{
    remove_session_call_ = true;
}

celeritas::server_network_type celeritas::mock_parameters_session::get_server_network_type() const
{
    return server_network_type::tcp;
}

void celeritas::mock_parameters_session::set_instance_id(const std::string& instance_id)
{
    instance_id_ = instance_id;
}

std::string celeritas::mock_parameters_session::get_instance_id() const
{
    return instance_id_;
}

celeritas::session::any_io_executor celeritas::mock_parameters_session::get_any_io_executor()
{
    if (executor_ != nullptr)
    {
        return executor_;
    }

    throw celeritas_error{ "io executor not support" };
}

bool celeritas::mock_parameters_session::is_write_immediately_call() const
{
    return write_immediately_call_;
}

bool celeritas::mock_parameters_session::is_remove_session_call() const
{
    return remove_session_call_;
}

int celeritas::mock_parameters_session::get_write_with_header_count() const
{
    return write_with_header_count;
}

celeritas::header celeritas::mock_parameters_session::get_last_header() const
{
    return last_header;
}

std::string celeritas::mock_parameters_session::get_remote_ip_address() const
{
    return "";
}

