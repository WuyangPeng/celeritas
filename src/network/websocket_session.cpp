#include "websocket_session.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "detail/websocket_session_handle_session.h"

#include <utility>

celeritas::websocket_session::websocket_session(boost::asio::ip::tcp::socket socket,
                                                long session_id,
                                                network_message_callback_weak_ptr network_message_callback,
                                                const std::string& game_server_id,
                                                listener_weak_ptr listener)
    : web_socket_{ std::move(socket) },
      session_id_{ session_id },
      network_message_callback_{ std::move(network_message_callback) },
      listener_{ std::move(listener) }
{
    set_option(game_server_id);
}

void celeritas::websocket_session::set_option(const std::string& game_server_id)
{
    // 配置 WebSocket 选项
    web_socket_.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    web_socket_.set_option(boost::beast::websocket::stream_base::decorator(
        [game_server_id](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " " + game_server_id);
        }));
}

void celeritas::websocket_session::start()
{
    // 启动主运行协程
    boost::asio::co_spawn(web_socket_.get_executor(),
                          [self = shared_from_this()] {
                              return self->run();
                          },
                          boost::asio::detached);
}

long celeritas::websocket_session::get_session_id() const noexcept
{
    return session_id_;
}

void celeritas::websocket_session::close_web_socket()
{
    // WebSocket 正常或异常关闭后，执行 TCP 层的关闭
    web_socket_.close(boost::beast::websocket::close_code::normal);

    LOG_CHANNEL(network_channel, info) << "WS Session [" << session_id_ << "] terminated.";

    if (const auto listener = listener_.lock();
        listener != nullptr)
    {
        listener->remove_session(session_id_);
    }
}

celeritas::websocket_session::void_awaitable_type celeritas::websocket_session::run()
{
    websocket_session_handle_session handle{ web_socket_, session_id_, network_message_callback_ };

    co_await handle.run();

    close_web_socket();
}