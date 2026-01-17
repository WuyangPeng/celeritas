#pragma once

#include "database/pool/connection_pool.tpp"
#include "database/pool/mock/mock_pool_database_session.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

#include <atomic>
#include <chrono>
#include <memory>
#include <vector>

namespace celeritas
{
    class connection_pool_fixture
    {
    public:
        using class_type = connection_pool_fixture;
        using io_context_type = boost::asio::io_context;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;
        using session_type = mock_pool_database_session;
        using pool_type = connection_pool<session_type>;
        using guard_type = database_session_guard<session_type>;
        using guard_awaitable = boost::asio::awaitable<guard_type>;
        using pool_shared_ptr = std::shared_ptr<pool_type>;
        using guard_container = std::vector<guard_type>;
        using guard_container_awaitable = boost::asio::awaitable<guard_container>;
        using void_awaitable = boost::asio::awaitable<void>;
        using milliseconds_type = std::chrono::milliseconds;
        using atomic_bool_shared_ptr = std::shared_ptr<std::atomic_bool>;

        connection_pool_fixture();

        void spawn(awaitable_function func);

        void run(awaitable_function func);

        [[nodiscard]] io_context_type& get_io_context();

        [[nodiscard]] static guard_awaitable check_get_session(const pool_shared_ptr& pool);

        [[nodiscard]] static guard_container_awaitable get_many_sessions(const pool_shared_ptr& pool, int count);

        [[nodiscard]] static void_awaitable async_wait(milliseconds_type duration);

        [[nodiscard]] void_awaitable check_wait_get_session(const pool_shared_ptr& pool, atomic_bool_shared_ptr& waiter_finished);

        void set_test_end(bool test_end);

    private:
        io_context_type io_context_;
        bool test_end_;
    };
}
