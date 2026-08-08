#include "buffer_consumer.h"
#include "network_session_helper_internal_constant.h"
#include "common/buffer/buffer_guard.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "network/core/network_message_callback.h"
#include "network/session_helper/detail/http_session_run.h"

#include <boost/beast.hpp>
#include <boost/url.hpp>

celeritas::http_session_run::http_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback)
    : base_type{}, socket_{ socket }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::http_session_run::do_start()
{
    boost::asio::co_spawn(socket_.get_executor(),
                          noexcept_safe_call_and_log_awaitable([self = shared_from_this()] {
                                                                   return self->run();
                                                               },
                                                               network_channel,
                                                               "http session run start error: "),
                          boost::asio::detached);
}

celeritas::session_run::void_awaitable_type celeritas::http_session_run::run()
{
    if (socket_.is_open())
    {
        try
        {
            co_return co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() == boost::asio::error::eof ||
                error.code() == boost::asio::error::connection_reset)
            {
                LOG_CHANNEL(network_channel, info) << "session closed gracefully.";
            }
            else
            {
                LOG_CHANNEL(network_channel, warning) << "session error: " << error.what();
            }
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "an unexpected error occurred: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "listener unknown error.";
        }
    }

    close_socket();
}

void celeritas::http_session_run::close_socket()
{
    session_callback_.remove_session(session_id_);

    LOG_CHANNEL(network_channel, trace) << "http listener close session id =" << session_id_;
}

celeritas::session_run::void_awaitable_type celeritas::http_session_run::handle_one_request_message()
{
    boost::beast::flat_buffer buffer{};

    boost::beast::http::request_parser<boost::beast::http::string_body> parser{};

    co_await boost::beast::http::async_read(socket_, buffer, parser, boost::asio::use_awaitable);

    buffer_consumer consume_guard{ buffer };

    const auto request = parser.release();

    const auto target = request.target();

    if (target == "/favicon.ico")
    {
        co_return;
    }

    LOG_CHANNEL(network_channel, trace) << "target:  " << target;

    const auto url_view = boost::urls::parse_relative_ref(target).value();

    const auto path = url_view.path();

    LOG_CHANNEL(network_channel, trace) << "path:  " << path;

    const auto params = url_view.params();

    const auto& body = request.body();

    if (!body.empty())
    {
        LOG_CHANNEL(network_channel, trace) << "body:  " << body;

        call_back(path, body);
    }
    else
    {
        LOG_CHANNEL(network_channel, trace) << "params:  " << params;

        call_back(path, params);
    }
}

celeritas::session_run::void_awaitable_type celeritas::http_session_run::handle_one_response_message(const std::string& path)
{
    boost::beast::flat_buffer buffer{};

    boost::beast::http::response_parser<boost::beast::http::string_body> parser{};

    co_await boost::beast::http::async_read(socket_, buffer, parser, boost::asio::use_awaitable);

    buffer_consumer consume_guard{ buffer };

    const auto response = parser.release();

    if (const auto status = response.result();
        status != boost::beast::http::status::ok)
    {
        LOG_CHANNEL(network_channel, error) << "http error: " << status << " - " << response.reason();
        co_return;
    }

    if (const auto content_type = response.find(boost::beast::http::field::content_type);
        content_type != response.end())
    {
        LOG_CHANNEL(network_channel, trace) << "Content-Type: " << content_type->value();
    }

    const auto& body_content = response.body();

    LOG_CHANNEL(network_channel, trace) << "response body size: " << body_content.size() << std::endl;

    LOG_CHANNEL(network_channel, trace) << "response body:" << body_content;

    call_back(path + response_suffix.data(), body_content);
}

void celeritas::http_session_run::call_back(const std::string& path, const urls_params_view_type& params)
{
    const auto session = get_session();
    if (!session)
    {
        return;
    }

    const auto callback = session_callback_.get_network_message_callback_shared_ptr();
    if (!callback)
    {
        return;
    }

    LOG_CHANNEL(network_channel, trace) << "session call back session id =" << session->get_session_id();
    callback->call_back(path, params, session);
}

void celeritas::http_session_run::call_back(const std::string& path, const std::string& params)
{
    const auto session = get_session();
    if (!session)
    {
        return;
    }

    const auto callback = session_callback_.get_network_message_callback_shared_ptr();
    if (!callback)
    {
        return;
    }

    callback->call_back(path, params, session);
}