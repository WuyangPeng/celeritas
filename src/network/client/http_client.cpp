#include "http_client.h"
#include "network/session/http_session.h"
#include "common/logging/logger.h"
#include "config/basic/server_network_type.h"

celeritas::http_client::http_client(const any_io_executor& any_io_executor,
                                    network_message_callback_weak_ptr callback,
                                    std::string game_server_id,
                                    std::string host,
                                    const int port,
                                    std::string server_type,
                                    std::string path)
    : base_type{},
      any_io_executor_{ any_io_executor },
      network_message_callback_{ std::move(callback) },
      game_server_id_{ std::move(game_server_id) },
      host_{ std::move(host) },
      port_{ port },
      server_type_{ std::move(server_type) },
      session_{},
      path_{ std::move(path) }
{
}

celeritas::http_client::session_waitable_type celeritas::http_client::connect()
{
    try
    {
        co_await do_connect();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(network_channel, error) << "connect " << host_ << ":" << port_ << " threw an exception: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(network_channel, fatal) << "connect threw an unknown exception";
    }

    co_return session_;
}

std::string celeritas::http_client::get_host() const
{
    return host_;
}

int celeritas::http_client::get_port() const
{
    return port_;
}

std::string celeritas::http_client::get_server_type() const
{
    return server_type_;
}

celeritas::listener_sessions::network_message_callback_weak_ptr celeritas::http_client::get_network_message_callback()
{
    return network_message_callback_;
}

celeritas::http_client::void_waitable_type celeritas::http_client::write_immediately(const std::string& response) const
{
    if (is_open())
    {
        co_await session_->write_immediately(response);
    }

    if (is_open())
    {
        co_await session_->start_awaitable();
    }

    if (is_open())
    {
        session_->remove_session();
    }
}

bool celeritas::http_client::is_open() const
{
    return session_ != nullptr && session_->is_open();
}

void celeritas::http_client::remove_session(const int64_t session_id)
{
    if (session_id == session_id_)
    {
        session_->stop();
        session_.reset();
    }
}

void celeritas::http_client::stop()
{
    remove_session(session_id_);
}

celeritas::http_client::basic_resolver_results_waitable_type celeritas::http_client::get_end_points() const
{
    boost::asio::ip::tcp::resolver resolver{ any_io_executor_ };

    // 异步解析主机名
    auto result = co_await resolver.async_resolve(host_, std::to_string(port_), boost::asio::as_tuple(boost::asio::use_awaitable));
    if (const auto error = std::get<0>(result))
    {
        throw boost::system::system_error(error);
    }

    co_return std::get<1>(result);
}

celeritas::http_client::void_waitable_type celeritas::http_client::do_connect()
{
    const auto endpoints = co_await get_end_points();

    socket_type socket{ any_io_executor_ };

    // 异步连接到解析出的端点
    co_await boost::asio::async_connect(socket, endpoints, boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, info) << "successfully connected to " << host_ << ":" << port_;

    // 创建一个新的会话并返回
    session_ = std::make_shared<session_type>(std::move(socket),
                                              server_network_type::http,
                                              session_id_,
                                              game_server_id_,
                                              session_callback{ shared_from_this(), network_message_callback_ },
                                              false,
                                              host_,
                                              path_);
}





