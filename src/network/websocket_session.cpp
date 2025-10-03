#include "websocket_session.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "detail/websocket_session_handle_session.h"

celeritas::websocket_session::websocket_session(socket_type socket,
                                                const int64_t session_id,
                                                const std::string& game_server_id,
                                                session_callback session_callback)
    : base_type{ session_id, std::move(session_callback) },
      web_socket_{ std::move(socket) },
      websocket_session_write_{ web_socket_ }
{
    set_option(game_server_id);
}

void celeritas::websocket_session::set_option(const std::string& game_server_id)
{
    // 配置 WebSocket 选项
    web_socket_.set_option(beast_websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    web_socket_.set_option(beast_websocket::stream_base::decorator(
        [game_server_id](beast_websocket::response_type& response) {
            response.set(boost::beast::http::field::server, std::string{ BOOST_BEAST_VERSION_STRING } + " " + game_server_id);
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

celeritas::websocket_session::void_awaitable_type celeritas::websocket_session::run()
{
    websocket_session_handle_session handle{ web_socket_, get_session_id(), get_network_message_callback() };

    co_await handle.run();

    close_web_socket();
}

void celeritas::websocket_session::write(buffer_guard data)
{
    websocket_session_write_.write(std::move(data));
}

void celeritas::websocket_session::close_web_socket()
{
    // WebSocket 正常或异常关闭后，执行 TCP 层的关闭
    boost::system::error_code error_code{};
    web_socket_.close(beast_websocket::close_code::normal, error_code);

    if (error_code)
    {
        LOG_CHANNEL(network_channel, info) << "WS Session [" << get_session_id() << "] terminated error, code = " << error_code;
    }
    else
    {
        LOG_CHANNEL(network_channel, info) << "WS Session [" << get_session_id() << "] terminated.";
    }

    remove_session();
}