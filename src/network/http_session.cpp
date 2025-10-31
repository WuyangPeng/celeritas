#include "http_session.h"
#include "common/logger.h"
#include "common/noexcept_safe_call_and_log.h"
#include "detail/http_request_session_write.h"
#include "detail/http_response_session_write.h"
#include "detail/http_session_run.h"
#include "detail/http_session_write.h"

celeritas::http_session::http_session(socket_type socket,
                                      int64_t session_id,
                                      std::string game_server_id,
                                      session_callback session_callback,
                                      const bool is_server,
                                      std::string host)
    : base_type{ session_id, std::move(session_callback) },
      socket_{ std::move(socket) },
      http_run_{ std::make_shared<http_session_run>(socket_, session_id, get_session_callback()) },
      http_write_{ get_session_write(socket_, is_server, std::move(host)) },
      game_server_id_{ std::move(game_server_id) }
{
}

celeritas::http_session::~http_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   this->stop();
                               },
                               network_channel,
                               "closed http session error: ");
}

void celeritas::http_session::start()
{
    http_run_->start(shared_from_this());
}

bool celeritas::http_session::is_open() const
{
    return socket_.is_open();
}

void celeritas::http_session::stop()
{
    if (is_open())
    {
        boost::system::error_code error_code{};
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error_code);

        if (error_code)
        {
            LOG_CHANNEL(network_channel, info) << "http socket session [" << get_session_id() << "] terminated error, code = " << error_code.message();
        }
        else
        {
            LOG_CHANNEL(network_channel, info) << "http socket session [" << get_session_id() << "] terminated success.";
        }
    }
}

void celeritas::http_session::do_write(buffer_guard data)
{
    http_write_->write(std::move(data));
}

celeritas::http_session::session_write_shared_ptr celeritas::http_session::get_session_write(socket_type& socket, const bool is_server, std::string host)
{
    if (is_server)
        return std::make_shared<http_response_session_write>(socket);
    else
        return std::make_shared<http_request_session_write>(socket, std::move(host));
}

