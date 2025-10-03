#include "websocket_session_handle_one_message.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::websocket_session_handle_one_message::websocket_session_handle_one_message(web_socket_stream_type& web_socket, int64_t session_id, session_callback callback)
    : web_socket_{ web_socket }, session_id_{ session_id }, callback_{ std::move(callback) }
{
}

celeritas::websocket_session_handle_one_message::void_awaitable_type celeritas::websocket_session_handle_one_message::run()
{
    boost::beast::flat_buffer buffer{};
    while (web_socket_.is_open())
    {
        // 异步读取数据帧
        co_await web_socket_.async_read(buffer, boost::asio::use_awaitable);

        /* const auto callback = callback_.lock();
         if (callback != nullptr)
         {
             // callback->call_back();
         }*/

        buffer.consume(buffer.size());
    }
}