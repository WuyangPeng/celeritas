#include "network_internal_fwd.h"
#include "websocket_session_write.h"
#include "common/logger.h"

#include <boost/asio.hpp>

celeritas::websocket_session_write::websocket_session_write(web_socket_stream_type& web_socket)
    : base_type{}, web_socket_{ web_socket }, write_queue_{}, write_mutex_{}
{
}

void celeritas::websocket_session_write::write(buffer_guard data)
{
    if (write_buffer_guard(std::move(data)))
    {
        co_spawn_write();
    }
}

celeritas::session_write::void_awaitable_type celeritas::websocket_session_write::do_write()
{
    while (web_socket_.is_open())
    {
        try
        {
            if (const auto result = co_await do_one_write();
                !result)
            {
                co_return;
            }
        }
        catch (const boost::system::system_error& error)
        {
            LOG_CHANNEL(network_channel, warning) << "Write error: " << error.what();
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "Write unknown error: " << error.what();
            break;
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            break;
        }
    }
}

celeritas::session_write::void_awaitable_type celeritas::websocket_session_write::write_immediately(buffer_guard data)
{
    write_buffer_guard(std::move(data));

    co_await do_write();
}

bool celeritas::websocket_session_write::is_full()
{
    std::lock_guard lock{ write_mutex_ };

    if (write_queue_.size() > max_queue_size)
    {
        return true;
    }

    return false;
}

celeritas::websocket_session_write::bool_awaitable_type celeritas::websocket_session_write::do_one_write()
{
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return false; // 队列为空，退出协程
    }

    auto buffer_guard = std::move(*optional_buffer_guard);

    co_await web_socket_.async_write(boost::asio::const_buffer(buffer_guard.get(), buffer_guard.get_effective_size()), boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";

    co_return true;
}

celeritas::websocket_session_write::buffer_guard_optional_type celeritas::websocket_session_write::get_next_write_buffer()
{
    std::lock_guard lock{ write_mutex_ };

    if (write_queue_.empty())
    {
        return std::nullopt; // 队列为空，返回一个空对象
    }

    auto buffer = std::move(write_queue_.front());
    write_queue_.pop_front();

    return buffer;
}

void celeritas::websocket_session_write::co_spawn_write()
{
    // 如果发送协程没有在运行，就启动它
    co_spawn(web_socket_.get_executor(), [self = this->shared_from_this()] {
                 return self->do_write();
             },
             boost::asio::detached);
}

bool celeritas::websocket_session_write::write_buffer_guard(buffer_guard data)
{
    std::lock_guard lock{ write_mutex_ };
    write_queue_.emplace_back(std::move(data));

    if (write_queue_.size() == 1)
    {
        return true;
    }

    return false;
}