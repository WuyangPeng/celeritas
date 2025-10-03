#pragma once

#include "network_message_callback.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace celeritas
{
    class http_session : public std::enable_shared_from_this<http_session>
    {
    public:
        using class_type = http_session;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        explicit http_session(boost::asio::ip::tcp::socket socket,
                              long session_id,
                              const network_message_callback_weak_ptr& callback);

        // 启动会话处理协程
        void start();

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type run();

        // 处理 HTTP 请求并生成响应
        [[nodiscard]] boost::beast::http::response<boost::beast::http::string_body> handle_request(boost::beast::http::request<boost::beast::http::string_body>&& req);

        boost::asio::ip::tcp::socket socket_;
        long session_id_;
        network_message_callback_weak_ptr network_message_callback_;
    };
}

