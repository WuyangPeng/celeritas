#include "websocket_session_handle_session.h"
#include "websocket_session_run.h"
#include "common/logger.h"

celeritas::websocket_session_run::websocket_session_run(web_socket_stream_type& web_socket, int64_t session_id, session_callback session_callback)
    : base_type{}, web_socket_{ web_socket }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::websocket_session_run::do_start()
{
    // 启动主运行协程
    boost::asio::co_spawn(web_socket_.get_executor(),
                          [self = shared_from_this()] {
                              return self->run();
                          },
                          boost::asio::detached);
}

celeritas::session_run::void_awaitable_type celeritas::websocket_session_run::run()
{
    websocket_session_handle_session handle{ web_socket_, session_id_, session_callback_.get_network_message_callback(), get_session() };

    co_await handle.run();

    close_web_socket();
}

void celeritas::websocket_session_run::close_web_socket()
{
    // WebSocket 正常或异常关闭后，执行 TCP 层的关闭
    boost::system::error_code error_code{};
    web_socket_.close(beast_websocket::close_code::normal, error_code);

    if (error_code)
    {
        LOG_CHANNEL(network_channel, info) << "web socket session [" << session_id_ << "] terminated error, code = " << error_code.message();
    }
    else
    {
        LOG_CHANNEL(network_channel, info) << "web socket session [" << session_id_ << "] terminated.";
    }

    session_callback_.remove_session(session_id_);
}