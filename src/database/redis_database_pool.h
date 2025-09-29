#pragma once

#include "database_pool.h"
#include "redis_database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <deque>
#include <mutex>
#include <string>

namespace celeritas
{
    class redis_database_pool : public database_pool
    {
    public:
        using class_type = redis_database_pool;

        using awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<redis_database_session>;
        using session_awaitable_type = boost::asio::awaitable<session_shared_ptr>;

        redis_database_pool(boost::asio::io_context& io_context,
                            const std::string_view& host,
                            uint16_t port,
                            const std::string_view& password,
                            int min_connections,
                            int max_connections);

        // 异步初始化连接池
        [[nodiscard]] awaitable_type async_initialize() override;

        // 异步获取一个数据库会话
        [[nodiscard]] session_awaitable_type async_get_session();

        // 释放一个数据库会话
        void release_session(const session_shared_ptr& session);

    private:
        using session_container_type = std::deque<session_shared_ptr>;
        using waiter_type = boost::asio::any_completion_handler<void(session_shared_ptr)>;

        [[nodiscard]] awaitable_type async_one_initialize();

        boost::asio::io_context& io_context_;
        std::string host_;
        uint16_t port_;
        std::string password_;

        session_container_type sessions_;
        std::mutex mutex_;
        std::deque<waiter_type> waiters_;
        int connections_;
        int min_connections_;
        int max_connections_;
    };
}
