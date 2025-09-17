#include "common/logger.h"
#include "message_header.h"
#include "proto/common/common.pb.h"
#include "session.h"

celeritas::session::session(socket_type socket)
    : socket_{ std::move(socket) }
{
}

void celeritas::session::start()
{
    co_spawn(socket_.get_executor(), [self = shared_from_this()] {
        return self->handle_session();
    },
             boost::asio::detached);
}

celeritas::session::awaitable_type celeritas::session::handle_session()
{
    for (;;)
    {
        try
        {
            co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() != boost::asio::error::eof &&
                error.code() != boost::asio::error::connection_reset)
            {
                LOG(warning) << "Session error: " << error.what();
            }
        }
    }
}

celeritas::session::awaitable_type celeritas::session::handle_one_message()
{
    // 读取消息头
    message_header header{};
    co_await async_read(socket_,
                        boost::asio::buffer(&header, sizeof(header)),
                        boost::asio::use_awaitable);

    // 转换字节序
    header.header_type = ntohl(header.header_type);
    header.header_size = ntohl(header.header_size);
    header.body_size = ntohl(header.body_size);
    const auto size = header.header_size + header.body_size;
    if (size == 0)
    {
        co_return;
    }

    constexpr auto max_message_size = 16 * 1024 * 1024;  // 16 MB
    if (header.header_size > 0xFF || header.body_size > max_message_size)
    {
        LOG(error) << "oversized msg, drop connection";
        co_return;
    }

    // 读取消息体
    std::vector<char> data(size);
    co_await async_read(socket_,
                        boost::asio::buffer(data, size),
                        boost::asio::use_awaitable);

    // 日志
    LOG(debug) << "Received message of type: "
               << header.header_type
               << ",header size:"
               << header.header_size
               << ",body size:"
               << header.body_size;

    // 按类型反序列化
    if (header.header_type == common::client)
    {
        common::client_message_header proto_message{};
        proto_message.ParseFromArray(data.data(), header.header_size);
    }
    else if (header.header_type == common::server)
    {
        common::server_message_header proto_message{};
        proto_message.ParseFromArray(data.data(), header.header_size);
    }
}
