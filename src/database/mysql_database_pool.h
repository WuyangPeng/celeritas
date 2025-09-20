#pragma once

#include "mysql_database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <string>

namespace celeritas
{
    class mysql_database_pool
    {
    public:
        using class_type = mysql_database_pool;
        using awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<mysql_database_session>;
        using session_awaitable_type = boost::asio::awaitable<session_shared_ptr>;

        mysql_database_pool(boost::asio::io_context& io_context,
                            std::string host,
                            uint16_t port,
                            std::string user,
                            std::string password,
                            std::string db_name,
                            size_t pool_size,
                            boost::asio::ssl::context* ssl_context = nullptr);

        // 异步初始化连接池
        [[nodiscard]] awaitable_type async_initialize();

        // 异步获取一个数据库会话
        [[nodiscard]] session_awaitable_type async_get_session();

        // 释放一个数据库会话
        void release_session(const session_shared_ptr& session);

    private:
        using session_container_type = std::deque<session_shared_ptr>;

        boost::asio::io_context& io_context_;
        boost::asio::ssl::context* ssl_context_;
        const std::string host_;
        uint16_t port_;
        const std::string user_;
        const std::string password_;
        const std::string db_name_;

        session_container_type sessions_;
        std::mutex mutex_;
        std::condition_variable condition_variable_;
        size_t pool_size_;
    };
}
