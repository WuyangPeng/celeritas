#include "http_listener.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::http_listener::http_listener(boost::asio::io_context& io_context, int port, const network_message_callback_shared_ptr& callback)
    : io_context_{ io_context },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      network_message_callback_{ callback },
      is_running_{ true },
      sessions_{},
      session_id_{ 0 }
{
    // 允许地址重用
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

    LOG_CHANNEL(network_channel, info) << "HTTP Listening on port " << port << "...";
}

void celeritas::http_listener::start()
{
    boost::asio::co_spawn(io_context_, [this] {
        return accept_connections();
    }, boost::asio::detached);
}

void celeritas::http_listener::stop()
{
    is_running_ = false;

    // 取消 acceptor，模仿 tcp_listener 的实现方式
    boost::system::error_code error_code{};
    acceptor_.cancel(error_code);
    if (error_code)
    {
        LOG_CHANNEL(network_channel, warning) << "Failed to cancel acceptor: " << error_code.message();
    }
}

void celeritas::http_listener::remove_session(int64_t session_id)
{
    sessions_.erase(session_id);
}

celeritas::http_listener::void_awaitable_type celeritas::http_listener::accept_connections()
{
    while (is_running_)
    {
        try
        {
            co_await handle_connection();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() != boost::asio::error::operation_aborted)
            {
                LOG_CHANNEL(network_channel, warning) << "HTTP Listener error: " << error.what();
            }
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, warning) << "HTTP Listener exception: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, error) << "HTTP Listener unknown error.";
        }
    }
    LOG_CHANNEL(network_channel, info) << "HTTP Listener stopped.";
}

celeritas::http_listener::void_awaitable_type celeritas::http_listener::handle_connection()
{
    // 等待新连接
    auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));

    if (auto error = std::get<0>(result))
    {
        // 忽略操作被取消的错误
        if (error != boost::asio::error::operation_aborted)
        {
            LOG_CHANNEL(network_channel, warning) << "HTTP Listener accept error: " << error.message();
        }
    }
    else
    {
        // 成功接受连接
        auto socket = std::move(std::get<1>(result));
        long current_session_id = ++session_id_;

        LOG_CHANNEL(network_channel, info) << "Accepted new HTTP connection [" << current_session_id << "] from: " << socket.remote_endpoint();

        // 创建新的 http_session
        auto new_session = std::make_shared<session_type>(std::move(socket), current_session_id, network_message_callback_);

        // 将 session 存储起来，防止被销毁
        sessions_[current_session_id] = new_session;

        // 启动 http_session 协程
        new_session->start();
    }
}