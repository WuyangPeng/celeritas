#include "buffer_consumer.h"
#include "websocket_session_handle_one_message.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "network/message_header.h"

celeritas::websocket_session_handle_one_message::websocket_session_handle_one_message(web_socket_stream_type& web_socket, const int64_t session_id, network_message_callback_weak_ptr callback, session_weak_ptr session)
    : web_socket_{ web_socket }, session_id_{ session_id }, callback_{ std::move(callback) }, session_{ std::move(session) }
{
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::run()
{
    flat_buffer_type buffer{};
    while (web_socket_.is_open())
    {
        try
        {
            co_await do_run(buffer);
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "websocket session [" << session_id_ << "] run error: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "websocket session [" << session_id_ << "] run unknown error.";
            break;
        }
    }
}

void celeritas::websocket_session_handle_one_message::call_back(const message_header& message_header, buffer_guard buffer_guard)
{
    const auto session = session_.lock();

    if (const auto callback = callback_.lock();
        callback != nullptr && session != nullptr)
    {
        callback->call_back(message_header, std::move(buffer_guard), session);
    }
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::do_run(flat_buffer_type& buffer)
{
    // 异步读取数据帧
    co_await web_socket_.async_read(buffer, boost::asio::use_awaitable);

    buffer_consumer consume_guard{ buffer };

    const auto payload_size = buffer.size();
    const auto self_size = message_header::get_self_size();

    if (payload_size < self_size)
    {
        LOG_CHANNEL(network_channel, error) << "websocket frame too small for header";
        co_return;
    }

    const auto payload_data = std::span{ reinterpret_cast<const char*>(buffer.data().data()), payload_size };

    const auto header_view = payload_data.subspan(0, self_size);
    message_header base{};
    base.set_span(header_view);
    base.network_to_host();

    const auto total_size = base.get_total_size();

    if (payload_size < total_size + self_size)
    {
        LOG_CHANNEL(network_channel, error) << "websocket frame incomplete";
        co_return;
    }

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size), total_size };

    const auto body_view = payload_data.subspan(self_size, total_size);
    buffer_guard.set(body_view);

    call_back(base, std::move(buffer_guard));

    co_return;
}