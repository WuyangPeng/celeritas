#include "common/logger.h"
#include "session_base.tpp"
#include "tcp_client.h"

celeritas::tcp_client::tcp_client(boost::asio::io_context& io_context, message_handler_type handler)
    : io_context_{ io_context }, message_handler_{ std::move(handler) }
{
}

celeritas::tcp_client::awaitable_type celeritas::tcp_client::connect(const std::string& host, uint16_t port)
{
    boost::asio::ip::tcp::resolver resolver(io_context_);
    socket_type socket(io_context_);

    try
    {
        // 异步解析主机名
        auto result = co_await resolver.async_resolve(host, std::to_string(port), boost::asio::as_tuple(boost::asio::use_awaitable));
        if (auto error = std::get<0>(result))
        {
            LOG_CHANNEL(network_channel, warning) << "connected error: " << error.message();
        }
        else
        {
            auto endpoints = std::get<1>(result);
            // 异步连接到解析出的端点
            co_await boost::asio::async_connect(socket, endpoints, boost::asio::use_awaitable);

            LOG_CHANNEL(network_channel, info) << "Successfully connected to " << host << ":" << port;

            // 创建一个新的会话并返回
            co_return std::make_shared<session_type>(std::move(socket), message_handler_);
        }
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(network_channel, error) << "Failed to connect to " << host << ":" << port << ". Error: " << error.what();
        co_return nullptr;
    }
}