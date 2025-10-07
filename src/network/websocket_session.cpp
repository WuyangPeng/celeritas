#include "websocket_session.h"
#include "common/logger.h"
#include "detail/websocket_session_handle_session.h"
#include "detail/websocket_session_run.h"
#include "detail/websocket_session_write.h"

celeritas::websocket_session::websocket_session(socket_type socket,
                                                const int64_t session_id,
                                                const std::string& game_server_id,
                                                session_callback session_callback)
    : base_type{ session_id, std::move(session_callback) },
      websocket_{ std::move(socket) },
      websocket_write_{ std::make_shared<websocket_session_write>(websocket_) },
      websocket_run_{ std::make_shared<websocket_session_run>(websocket_, session_id, get_session_callback()) }
{
    set_option(game_server_id);
}

void celeritas::websocket_session::set_option(const std::string& game_server_id)
{
    // 配置 WebSocket 选项
    websocket_.set_option(beast_websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    websocket_.set_option(beast_websocket::stream_base::decorator(
        [game_server_id](beast_websocket::response_type& response) {
            response.set(boost::beast::http::field::server, std::string{ BOOST_BEAST_VERSION_STRING } + " " + game_server_id);
        }));
}

void celeritas::websocket_session::start()
{
    websocket_run_->start();
}

void celeritas::websocket_session::write(buffer_guard data)
{
    websocket_write_->write(std::move(data));
}
