#include "session_base.tpp"
#include "tcp_client.h"
#include "common/logger.h"

celeritas::tcp_client::tcp_client(io_context_type& io_context, network_message_callback_weak_ptr callback, std::string game_server_id, std::string host, int port, std::string server_type)
    : base_type{},
      io_context_{ io_context },
      network_message_callback_{ std::move(callback) },
      game_server_id_{ std::move(game_server_id) },
      session_id_{ 0 },
      session_{},
      host_{ std::move(host) },
      port_{ port },
      server_type_{ std::move(server_type) }
{
}

celeritas::tcp_client::session_waitable_type celeritas::tcp_client::connect()
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

bool celeritas::tcp_client::is_open() const
{
    return session_ != nullptr && session_->is_open();
}

std::string celeritas::tcp_client::get_host() const
{
    return host_;
}

int celeritas::tcp_client::get_port() const
{
    return port_;
}

std::string celeritas::tcp_client::get_server_type() const
{
    return server_type_;
}

celeritas::listener_sessions::network_message_callback_weak_ptr celeritas::tcp_client::get_network_message_callback()
{
    return network_message_callback_;
}

void celeritas::tcp_client::write(const header& header, const google::protobuf::Message& request)
{
    if (is_open())
    {
        session_->write(header, request);
    }
}

celeritas::tcp_client::void_waitable_type celeritas::tcp_client::do_connect()
{
    boost::asio::ip::tcp::resolver resolver{ io_context_ };
    socket_type socket{ io_context_ };

    // 异步解析主机名
    auto result = co_await resolver.async_resolve(host_, std::to_string(port_), boost::asio::as_tuple(boost::asio::use_awaitable));
    if (auto error = std::get<0>(result))
    {
        throw boost::system::system_error(error);
    }

    auto endpoints = std::get<1>(result);
    // 异步连接到解析出的端点
    co_await boost::asio::async_connect(socket, endpoints, boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, info) << "Successfully connected to " << host_ << ":" << port_;

    // 创建一个新的会话并返回
    session_ = std::make_shared<session_type>(std::move(socket),
                                              ++session_id_,
                                              game_server_id_,
                                              session_callback{ shared_from_this(), network_message_callback_ });

    session_->start();
}



