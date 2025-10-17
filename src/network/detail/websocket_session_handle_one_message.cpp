#include "websocket_session_handle_one_message.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "common/common_fwd.h"
#include "network/message_header.h"

celeritas::websocket_session_handle_one_message::websocket_session_handle_one_message(web_socket_stream_type& web_socket, int64_t session_id, network_message_callback_weak_ptr callback, session_weak_ptr session)
    : web_socket_{ web_socket }, session_id_{ session_id }, callback_{ std::move(callback) }, session_{ std::move(session) }
{
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::run()
{
    boost::beast::flat_buffer buffer{};
    while (web_socket_.is_open())
    {
        // 异步读取数据帧
        co_await web_socket_.async_read(buffer, boost::asio::use_awaitable);

        const auto payload_size = buffer.size();
        auto* payload_data = static_cast<const uint8_t*>(buffer.data().data());

        message_header base{};
        if (payload_size < sizeof(base))
        {
            LOG_CHANNEL(network_channel, error) << "websocket frame too small for header";
            buffer.consume(payload_size);
            continue;
        }

        std::memcpy(&base, payload_data, sizeof(base));
        base.network_to_host();

        const auto total_size = base.get_total_size();

        if (payload_size < total_size - sizeof(base))
        {
            LOG_CHANNEL(network_channel, error) << "websocket frame incomplete";
            buffer.consume(payload_size);
            continue;
        }

        buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
        buffer_guard.set_effective_size(total_size);
        std::memcpy(buffer_guard.get(), payload_data, total_size);

        auto session = session_.lock();

        if (const auto callback = callback_.lock();
            callback != nullptr && session != nullptr)
        {
            callback->call_back(base, std::move(buffer_guard), session);
        }

        buffer.consume(buffer.size());
    }
}