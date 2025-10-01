#include "websocket_session.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::websocket_session::websocket_session(boost::asio::ip::tcp::socket socket, long session_id, const network_message_callback_shared_ptr& callback)
    : ws_{ std::move(socket) },
      session_id_{ session_id },
      network_message_callback_{ callback }
{
    // 配置 WebSocket 选项 (可选)
    ws_.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    ws_.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " celeritas-ws-server");
        }));
}

void celeritas::websocket_session::start()
{
    // 启动主运行协程
    boost::asio::co_spawn(ws_.get_executor(), [self = shared_from_this()] {
        return self->run();
    }, boost::asio::detached);
}

void celeritas::websocket_session::write(std::string message)
{
    std::lock_guard<std::mutex> lock(write_mutex_);
    write_queue_.push_back(std::move(message));

    // 如果队列中只有一个消息（即刚刚添加的），我们需要启动写入协程
    // 否则，写入协程已经在运行中。
    if (write_queue_.size() == 1)
    {
        boost::asio::co_spawn(ws_.get_executor(), [self = shared_from_this()] {
            return self->do_write();
        }, boost::asio::detached);
    }
}

celeritas::websocket_session::void_awaitable_type celeritas::websocket_session::do_write()
{
    while (ws_.is_open())
    {
        std::string message;

        {
            std::lock_guard<std::mutex> lock(write_mutex_);
            if (write_queue_.empty())
            {
                co_return; // 队列为空，退出写入协程
            }
            message = std::move(write_queue_.front());
            write_queue_.pop_front();
        }

        try
        {
            // 设置发送文本帧
            ws_.text(true);
            // 异步发送消息
            co_await ws_.async_write(boost::asio::buffer(message), boost::asio::use_awaitable);

            LOG_CHANNEL(network_channel, debug) << "WS successfully wrote " << message.size() << " bytes.";
        }
        catch (const boost::system::system_error& error)
        {
            LOG_CHANNEL(network_channel, warning) << "WS Write error: " << error.what();
            break;
        }
    }
}

celeritas::websocket_session::void_awaitable_type celeritas::websocket_session::run()
{
    try
    {
        // 1. HTTP 握手
        // 这是 WebSocket 独有的步骤，它将会把原始的 TCP 连接升级为 WebSocket 连接
        /*  co_await ws_.async_accept(boost::asio::use_awaitable);

        LOG_CHANNEL(network_channel, info) << "WS Session [" << session_id_ << "] upgraded to WebSocket.";

        // 2. 读消息循环
        boost::beast::flat_buffer buffer;
          while (ws_.is_open())
          {
              // 异步读取数据帧
              co_await ws_.async_read(buffer, boost::asio::use_awaitable);

              // 处理接收到的消息：这里我们简单地回显 (echo) 消息
              if (ws_.got_text())
              {
                  // 将数据转为字符串
                  std::string received_message = boost::beast::buffers_to_string(buffer.data());

                  // 示例：将消息回显给客户端
                  write(std::move(received_message));

                  // TODO: 实际应用中，您应该在这里调用 network_message_callback_ 处理业务逻辑
              }
              // 清空 buffer 以准备下一次读取
              buffer.consume(buffer.size());
          }*/
    }
    catch (const boost::system::system_error& error)
    {
        if (error.code() != boost::beast::websocket::error::closed &&
            error.code() != boost::asio::error::operation_aborted)
        {
            LOG_CHANNEL(network_channel, warning) << "WS Session [" << session_id_ << "] run error: " << error.what();
        }
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(network_channel, error) << "WS Session unknown error: " << error.what();
    }

    // WebSocket 正常或异常关闭后，执行 TCP 层的关闭
    /*    auto result = co_await ws_.async_close(boost::beast::websocket::close_code::normal, boost::asio::as_tuple(boost::asio::use_awaitable));
        if (auto error = std::get<0>(result))
        {
            throw boost::system::system_error(error);
        }

        LOG_CHANNEL(network_channel, info) << "WS Session [" << session_id_ << "] terminated.";*/

    co_return;
}