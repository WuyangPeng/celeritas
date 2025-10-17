#include "session_base.tpp"
#include "tcp_client.h"
#include "common/logger.h"

celeritas::tcp_client::tcp_client(io_context_type& io_context, network_message_callback_weak_ptr callback, std::string game_server_id)
    : base_type{},
      io_context_{ io_context },
      network_message_callback_{ std::move(callback) },
      game_server_id_{ std::move(game_server_id) },
      session_id_{ 0 }
{
}

celeritas::tcp_client::session_waitable_type celeritas::tcp_client::connect(const std::string& host, int port)
{
    boost::asio::ip::tcp::resolver resolver{ io_context_ };
    socket_type socket{ io_context_ };

    // 异步解析主机名
    auto result = co_await resolver.async_resolve(host, std::to_string(port), boost::asio::as_tuple(boost::asio::use_awaitable));
    if (auto error = std::get<0>(result))
    {
        throw boost::system::system_error(error);
    }

    auto endpoints = std::get<1>(result);
    // 异步连接到解析出的端点
    co_await boost::asio::async_connect(socket, endpoints, boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, info) << "Successfully connected to " << host << ":" << port;

    // 创建一个新的会话并返回
    co_return std::make_shared<session_type>(std::move(socket),
                                             ++session_id_,
                                             game_server_id_,
                                             session_callback{ shared_from_this(), network_message_callback_ });
}

