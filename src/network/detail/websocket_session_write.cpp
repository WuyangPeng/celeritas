#include "websocket_session_write.h"
#include "common/logger.h"
#include "common/common_fwd.h"

#include <boost/asio.hpp>

celeritas::websocket_session_write::websocket_session_write(web_socket_stream_type& web_socket)
    : base_type{}, web_socket_{ web_socket }, write_queue_{}, write_mutex_{}
{
}

void celeritas::websocket_session_write::write(buffer_guard data)
{
    std::lock_guard lock{ write_mutex_ };
    write_queue_.emplace_back(std::move(data));

    // 如果发送协程没有在运行，就启动它
    if (write_queue_.size() == 1)
    {
        co_spawn(web_socket_.get_executor(), [self = this->shared_from_this()] {
                     return self->do_write();
                 },
                 boost::asio::detached);
    }
}

celeritas::session_write::void_awaitable_type celeritas::websocket_session_write::do_write()
{
    while (web_socket_.is_open())
    {
        try
        {
            co_await do_one_write();
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

celeritas::session_write::void_awaitable_type celeritas::websocket_session_write::do_one_write()
{
    // 调用新函数来获取数据，该函数内部处理了加锁和解锁
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return; // 队列为空，退出协程
    }
    auto buffer_guard = std::move(*optional_buffer_guard);

    co_await web_socket_.async_write(boost::asio::const_buffer(buffer_guard.get(), buffer_guard.get_effective_size()), boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";
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