#include "http_session_write.h"
#include "network_internal_fwd.h"
#include "common/logger.h"
#include "network/session_base.h"

#include <boost/asio.hpp>

celeritas::http_session_write::http_session_write(socket_type& socket, std::string host)
    : base_type{}, socket_{ socket }, write_queue_{}, write_mutex_{}, host_{ std::move(host) }, session_{}
{
}

void celeritas::http_session_write::write(buffer_guard data)
{
    if (write_buffer_guard(std::move(data)))
    {
        co_spawn_write();
    }
}

celeritas::session_write::void_awaitable_type celeritas::http_session_write::do_write()
{
    while (socket_.is_open())
    {
        try
        {
            if (const auto result = co_await do_one_write();
                !result)
            {
                if (const auto session_shared_ptr = session_.lock();
                    session_shared_ptr != nullptr)
                {
                    session_shared_ptr->remove_session();
                }
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

celeritas::session_write::void_awaitable_type celeritas::http_session_write::write_immediately(buffer_guard data, const session_weak_ptr& session)
{
    LOG_CHANNEL(network_channel, debug) << "http start wrote " << data.get_effective_size() << " bytes to client.";

    session_ = session;

    write_buffer_guard(std::move(data));

    co_await do_write();
}

bool celeritas::http_session_write::is_full()
{
    std::lock_guard lock{ write_mutex_ };

    if (write_queue_.size() > max_queue_size)
    {
        return true;
    }

    return false;
}

celeritas::http_session_write::bool_awaitable_type celeritas::http_session_write::do_one_write_response()
{
    // 调用新函数来获取数据，该函数内部处理了加锁和解锁
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return false; // 队列为空，退出协程
    }

    auto buffer_guard = std::move(*optional_buffer_guard);
    const auto body_size = buffer_guard.get_effective_size();

    boost::beast::http::response<boost::beast::http::buffer_body> response{ boost::beast::http::status::ok, 11, boost::beast::http::buffer_body::value_type{} };

    // 设置 HTTP 头部字段
    response.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(boost::beast::http::field::content_type, "text/html; charset=utf-8");
    response.set(boost::beast::http::field::connection, "keep-alive");

    // 设置 Content-Length (必须设置)
    response.content_length(body_size);

    // 这一步设置了响应体的内容和大小
    response.body().data = buffer_guard.get();
    response.body().size = body_size;
    response.body().more = false;

    co_await boost::beast::http::async_write(socket_, response, boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";

    co_return true;
}

celeritas::http_session_write::bool_awaitable_type celeritas::http_session_write::do_one_write_request(const std::string& path)
{
    // 调用新函数来获取数据，该函数内部处理了加锁和解锁
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return false; // 队列为空，退出协程
    }

    auto buffer_guard = std::move(*optional_buffer_guard);
    const auto body_size = buffer_guard.get_effective_size();

    // 构建 HTTP GET 请求

    // HTTP/1.1
    boost::beast::http::request<boost::beast::http::buffer_body> request{ boost::beast::http::verb::get, path, 11 };
    request.set(boost::beast::http::field::host, host_);
    request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(boost::beast::http::field::content_type, "application/octet-stream");

    // 必须设置 Content-Length
    request.content_length(body_size);

    request.body().data = buffer_guard.get();
    request.body().size = body_size;
    request.body().more = false;

    co_await boost::beast::http::async_write(socket_, request, boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";

    co_return true;
}

celeritas::http_session_write::buffer_guard_optional_type celeritas::http_session_write::get_next_write_buffer()
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

void celeritas::http_session_write::co_spawn_write()
{
    co_spawn(socket_.get_executor(), [self = this->shared_from_this()] {
                 return self->do_write();
             },
             boost::asio::detached);
}

bool celeritas::http_session_write::write_buffer_guard(buffer_guard data)
{
    std::lock_guard lock{ write_mutex_ };
    write_queue_.emplace_back(std::move(data));

    if (write_queue_.size() == 1)
    {
        return true;
    }

    return false;
}