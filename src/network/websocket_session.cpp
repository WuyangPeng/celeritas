#include "websocket_session.h"
#include "common/logger.h"
#include "common/noexcept_safe_call_and_log.h"
#include "detail/websocket_session_run.h"
#include "detail/websocket_session_write.h"

celeritas::websocket_session::websocket_session(socket_type socket,
                                                const int64_t session_id,
                                                const std::string& game_server_id,
                                                session_callback session_callback)
    : base_type{ session_id, std::move(session_callback) },
      websocket_{ std::move(socket) },
      websocket_run_{ std::make_shared<websocket_session_run>(websocket_, session_id, get_session_callback()) },
      websocket_write_{ std::make_shared<websocket_session_write>(websocket_) }
{
    set_option(game_server_id);
}

celeritas::websocket_session::~websocket_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   this->stop();
                               },
                               network_channel,
                               "closed websocket session error: ");
}

void celeritas::websocket_session::start()
{
    websocket_run_->start(shared_from_this());
}

celeritas::session::void_awaitable_type celeritas::websocket_session::start_awaitable()
{
    co_await websocket_run_->start_awaitable(shared_from_this());
}

void celeritas::websocket_session::stop()
{
    if (is_open())
    {
        // WebSocket 正常或异常关闭后，执行 TCP 层的关闭
        boost::system::error_code error_code{};
        websocket_.close(beast_websocket::close_code::normal, error_code);

        if (error_code)
        {
            LOG_CHANNEL(network_channel, info) << "web socket session [" << get_session_id() << "] terminated error, code = " << error_code.message();
        }
        else
        {
            LOG_CHANNEL(network_channel, info) << "web socket session [" << get_session_id() << "] terminated.";
        }
    }
}

bool celeritas::websocket_session::is_open() const
{
    return websocket_.is_open();
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

void celeritas::websocket_session::do_write(buffer_guard data)
{
    websocket_write_->write(std::move(data));
}

celeritas::session::void_awaitable_type celeritas::websocket_session::do_write_immediately(buffer_guard data)
{
    co_await websocket_write_->write_immediately(std::move(data));
}
