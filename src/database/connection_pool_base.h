#pragma once

#include "database_pool.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <deque>

namespace celeritas
{
    template <typename SessionType>
    class connection_pool_base final : public database_pool
    {
    public:
        using class_type = connection_pool_base;
        using base_type = database_pool;

        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<SessionType>;
        using session_awaitable_type = boost::asio::awaitable<session_shared_ptr>;

        connection_pool_base(io_context_type& io_context,
                             std::string host,
                             int port,
                             std::string user,
                             std::string password,
                             std::string db_name,
                             int min_connections,
                             int max_connections);

        connection_pool_base(io_context_type& io_context,
                             std::string host,
                             int port,
                             std::string user,
                             std::string password,
                             int min_connections,
                             int max_connections);

        connection_pool_base(io_context_type& io_context,
                             std::string uri,
                             std::string db_name,
                             int min_connections,
                             int max_connections);

        // 异步初始化连接池
        [[nodiscard]] void_awaitable_type async_initialize() override;

        // 异步获取一个数据库会话
        [[nodiscard]] session_awaitable_type async_get_session();

        // 释放一个数据库会话
        void release_session(const session_shared_ptr& session);

        void cleanup_database_by_duration() override;

        [[nodiscard]] bool_awaitable_type is_health() override;

    private:
        using session_container_type = std::deque<session_shared_ptr>;
        using waiter_type = boost::asio::any_completion_handler<void(session_shared_ptr)>;
        using self_shared_ptr = std::shared_ptr<base_type>;

        [[nodiscard]] void_awaitable_type async_one_initialize();

        [[nodiscard]] void_awaitable_type do_async_one_initialize();

        [[nodiscard]] session_shared_ptr try_get_existing_session();

        [[nodiscard]] session_awaitable_type async_initiate_session();

        io_context_type& io_context_;
        std::string host_;
        int port_;
        std::string user_;
        std::string password_;
        std::string uri_;
        std::string db_name_;

        session_container_type sessions_;
        std::mutex mutex_;
        std::deque<waiter_type> waiters_;
        std::atomic_int connections_;
        int min_connections_;
        int max_connections_;
    };
}