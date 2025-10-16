#include "http_session_run.h"
#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "common/common_fwd.h"
#include "network/message_header.h"
#include "network/network_message_callback.h"

#include <boost/beast.hpp>

celeritas::http_session_run::http_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback)
    : base_type{}, socket_{ socket }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::http_session_run::do_start()
{
    boost::asio::co_spawn(socket_.get_executor(),
                          [self = shared_from_this()] {
                              return self->run();
                          }, boost::asio::detached);
}

celeritas::session_run::void_awaitable_type celeritas::http_session_run::run()
{
    while (socket_.is_open())
    {
        try
        {
            co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() == boost::asio::error::eof ||
                error.code() == boost::asio::error::connection_reset)
            {
                LOG_CHANNEL(network_channel, info) << "Session closed gracefully.";
            }
            else
            {
                LOG_CHANNEL(network_channel, warning) << "Session error: " << error.what();
            }
            socket_.close();
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "An unexpected error occurred: " << error.what();
            socket_.close();
            break;
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            socket_.close();
            break;
        }
    }

    close_socket();
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

celeritas::session_run::void_awaitable_type celeritas::http_session_run::handle_one_message()
{
    boost::beast::flat_buffer buffer{};

    boost::beast::http::request_parser<boost::beast::http::string_body> parser{};

    co_await boost::beast::http::async_read(socket_, buffer, parser, boost::asio::use_awaitable);

    const auto payload_size = buffer.size();
    auto* payload_data = static_cast<const uint8_t*>(buffer.data().data());

    message_header base{};
    if (payload_size < sizeof(base))
    {
        LOG_CHANNEL(network_channel, error) << "http buffer too small for header";
        buffer.consume(payload_size);
        co_return;
    }

    std::memcpy(&base, payload_data, sizeof(base));
    base.network_to_host();

    const auto total_size = base.get_total_size();

    if (payload_size < total_size - sizeof(base))
    {
        LOG_CHANNEL(network_channel, error) << "http buffer incomplete";
        buffer.consume(payload_size);
        co_return;
    }

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
    buffer_guard.set_effective_size(total_size);
    std::memcpy(buffer_guard.get(), payload_data, total_size);

    auto session = get_session();
    if (const auto callback = session_callback_.get_network_message_callback().lock();
        callback != nullptr && session != nullptr)
    {
        callback->call_back(base, std::move(buffer_guard), session);
    }

    buffer.consume(buffer.size());
}