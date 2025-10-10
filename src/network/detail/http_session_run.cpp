#include "http_session_run.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::http_session_run::http_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback)
    : base_type{}, socket_{ socket }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::http_session_run::start()
{
    boost::asio::co_spawn(socket_.get_executor(),
                          [self = shared_from_this()] {
                              return self->run();
                          }, boost::asio::detached);
}

celeritas::session_run::void_awaitable_type celeritas::http_session_run::run()
{
    close_socket();

    co_return;
}

void celeritas::http_session_run::close_socket()
{
    boost::system::error_code error_code{};
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error_code);

    if (error_code)
    {
        LOG_CHANNEL(network_channel, info) << "http socket session [" << session_id_ << "] terminated error, code = " << error_code.message();
    }
    else
    {
        LOG_CHANNEL(network_channel, info) << "http socket session [" << session_id_ << "] terminated.";
    }

    session_callback_.remove_session(session_id_);
}