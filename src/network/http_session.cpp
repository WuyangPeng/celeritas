#include "http_session.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::http_session::http_session(boost::asio::ip::tcp::socket socket, long session_id, const network_message_callback_weak_ptr& callback)
    : socket_{ std::move(socket) },
      session_id_{ session_id },
      network_message_callback_{ callback }
{
}

void celeritas::http_session::start()
{
    boost::asio::co_spawn(socket_.get_executor(), [self = shared_from_this()] {
        return self->run();
    }, boost::asio::detached);
}

celeritas::http_session::void_awaitable_type celeritas::http_session::run()
{
    boost::system::error_code ec{};
    boost::beast::flat_buffer buffer{};

    while (socket_.is_open())
    {
        try
        {
            // 1. 读取 HTTP 请求 (使用 use_awaitable)
            boost::beast::http::request<boost::beast::http::string_body> req;
            co_await boost::beast::http::async_read(socket_, buffer, req, boost::asio::use_awaitable);

            // 2. 处理请求并获取响应
            auto res = handle_request(std::move(req));

            // 3. 写入响应
            boost::beast::http::serializer<false, boost::beast::http::string_body> sr{ res };
            co_await boost::beast::http::async_write(socket_, sr, boost::asio::use_awaitable);

            // 4. 检查是否需要关闭连接 (例如 HTTP/1.0 或带有 Connection: close)
            if (res.need_eof())
            {
                break;
            }
        }
        catch (const boost::system::system_error& error)
        {
            // 忽略正常的连接关闭错误
            if (error.code() != boost::beast::http::error::end_of_stream && error.code() != boost::asio::error::operation_aborted)
            {
                LOG_CHANNEL(network_channel, warning) << "HTTP Session error: " << error.what();
            }
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "HTTP Session unknown error: " << error.what();
            break;
        }
    }

    // 优雅关闭 socket (TCP level)
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
}

boost::beast::http::response<boost::beast::http::string_body> celeritas::http_session::handle_request(boost::beast::http::request<boost::beast::http::string_body>&& req)
{
    boost::beast::http::response<boost::beast::http::string_body> res{ boost::beast::http::status::ok, req.version() };
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);

    if (req.method() == boost::beast::http::verb::get)
    {
        if (req.target() == "/")
        {
            res.set(boost::beast::http::field::content_type, "text/html");
            res.body() = "<h1>Welcome to Celeritas HTTP Server</h1>";
        }
        else if (req.target() == "/ping")
        {
            res.set(boost::beast::http::field::content_type, "text/plain");
            res.body() = "pong";
        }
        else
        {
            res.result(boost::beast::http::status::not_found);
            res.set(boost::beast::http::field::content_type, "text/plain");
            res.body() = "Not Found.";
        }
    }
    else
    {
        res.result(boost::beast::http::status::bad_request);
        res.set(boost::beast::http::field::allow, "GET");
        res.set(boost::beast::http::field::content_type, "text/plain");
        res.body() = "Invalid request method. Only GET is supported.";
    }

    res.prepare_payload();
    return res;
}