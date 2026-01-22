#include "mock_session_base.h"

celeritas::mock_session_base::mock_session_base(any_io_executor executor, server_network_type server_network_type, int64_t session_id, session_callback session_callback)
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

void celeritas::mock_session_base::do_write(buffer_guard data)
{
}

celeritas::session::void_awaitable_type celeritas::mock_session_base::do_write_immediately(buffer_guard data)
{
    co_return;
}