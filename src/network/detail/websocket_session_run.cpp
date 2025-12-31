#include "websocket_session_handle_session.h"
#include "websocket_session_run.h"
#include "common/logging/logger.h"

celeritas::websocket_session_run::websocket_session_run(web_socket_stream_type& web_socket, const int64_t session_id, session_callback session_callback)
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
    session_callback_.remove_session(session_id_);
}