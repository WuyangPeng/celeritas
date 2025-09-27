#pragma once

#include "database_pool.h"
#include "mongo_database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <deque>
#include <mutex>
#include <string>

namespace celeritas
{
    class mongo_database_pool : public database_pool
    {
    public:
        using class_type = mongo_database_pool;

        using awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<mongo_database_session>;
        using session_awaitable_type = boost::asio::awaitable<session_shared_ptr>;

        mongo_database_pool(boost::asio::io_context& io_context,
                            const std::string_view& uri,
                            const std::string_view& db_name,
                            size_t pool_size);

        // 异步初始化连接池
        [[nodiscard]] awaitable_type async_initialize();

        // 异步获取一个数据库会话
        [[nodiscard]] session_awaitable_type async_get_session();

        // 释放一个数据库会话
        void release_session(const session_shared_ptr& session);

    private:
        using session_container_type = std::deque<session_shared_ptr>;
        using waiter_type = boost::asio::any_completion_handler<void(session_shared_ptr)>;

        boost::asio::io_context& io_context_;
        std::string uri_;
        std::string db_name_;

        session_container_type sessions_;
        std::mutex mutex_;
        std::deque<waiter_type> waiters_;
        size_t pool_size_;
    };
}
