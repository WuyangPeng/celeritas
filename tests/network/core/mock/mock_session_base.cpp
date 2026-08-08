#include "mock_session_base.h"
#include "common/buffer/buffer_guard.h"

celeritas::mock_session_base::mock_session_base(any_io_executor executor, const server_network_type server_network_type, const int64_t session_id, session_callback session_callback)
    : base_type{ server_network_type, session_id, std::move(session_callback) }, executor_{ std::move(executor) }
{
}

void celeritas::mock_session_base::start()
{
}

celeritas::session::void_awaitable_type celeritas::mock_session_base::start_awaitable()
{
    co_return;
}

bool celeritas::mock_session_base::is_open() const
{
    return true;
}

bool celeritas::mock_session_base::is_full() const
{
    return false;
}

void celeritas::mock_session_base::stop()
{
}

celeritas::mock_session_base::any_io_executor celeritas::mock_session_base::get_any_io_executor()
{
    return executor_;
}

bool celeritas::mock_session_base::was_do_write_called() const
{
    return do_write_called_;
}

bool celeritas::mock_session_base::was_do_write_immediately_called() const
{
    return do_write_immediately_called_;
}

size_t celeritas::mock_session_base::get_last_write_size() const
{
    return last_write_size_;
}

std::string celeritas::mock_session_base::get_remote_ip_address() const
{
    return "";
}

void celeritas::mock_session_base::do_write(buffer_guard data)
{
    do_write_called_ = true;
    last_write_size_ = data.get_effective_size();
}

celeritas::session::void_awaitable_type celeritas::mock_session_base::do_write_immediately(buffer_guard data)
{
    do_write_immediately_called_ = true;
    last_write_size_ = data.get_effective_size();
    co_return;
}
