#include "buffer_consumer.h"
#include "read_buffer_guard.h"
#include "websocket_session_handle_one_message.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "network/message_header.h"

celeritas::websocket_session_handle_one_message::websocket_session_handle_one_message(web_socket_stream_type& web_socket,
                                                                                      const int64_t session_id,
                                                                                      network_message_callback_weak_ptr callback,
                                                                                      session_weak_ptr session)
    : web_socket_{ web_socket }, session_id_{ session_id }, callback_{ std::move(callback) }, session_{ std::move(session) }
{
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::run() const
{
    flat_buffer_type buffer{};
    while (web_socket_.is_open())
    {
        co_await run(buffer);
    }
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::run(flat_buffer_type& buffer) const
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
    }
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::do_run(flat_buffer_type& buffer) const
{
    // 异步读取数据帧
    co_await web_socket_.async_read(buffer, boost::asio::use_awaitable);

    buffer_consumer consume_guard{ buffer };

    const auto payload_data = std::span{ static_cast<const char*>(buffer.data().data()), buffer.size() };

    read_buffer_guard read_buffer_guard{ payload_data };

    if (auto buffer_guard = read_buffer_guard.get_buffer_guard())
    {
        call_back(read_buffer_guard.get_message_header(), std::move(*buffer_guard));
    }
}

void celeritas::websocket_session_handle_one_message::call_back(const message_header& message_header, buffer_guard buffer_guard) const
{
    const auto session = session_.lock();

    if (const auto call_back = callback_.lock();
        call_back != nullptr && session != nullptr)
    {
        call_back->call_back(message_header, std::move(buffer_guard), session);
    }
}