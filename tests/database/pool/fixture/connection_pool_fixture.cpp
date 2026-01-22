#include "connection_pool_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/test/unit_test.hpp>

celeritas::connection_pool_fixture::connection_pool_fixture()
    : io_context_{},
      test_end_{ false }
{
}

void celeritas::connection_pool_fixture::spawn(awaitable_function func)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(func),
                                                               database_channel,
                                                               "connection pool fixture run error: "),
                          boost::asio::detached);
}

void celeritas::connection_pool_fixture::run(awaitable_function func)
{
    spawn(std::move(func));

    io_context_.run();
    io_context_.restart();
    BOOST_CHECK(test_end_);
}

celeritas::connection_pool_fixture::io_context_type& celeritas::connection_pool_fixture::get_io_context()
{
    return io_context_;
}

celeritas::connection_pool_fixture::guard_awaitable celeritas::connection_pool_fixture::check_get_session(const pool_shared_ptr& pool)
{
    auto session_guard = co_await pool->async_get_session();
    BOOST_TEST(session_guard.get_session() != nullptr);
    co_return session_guard;
}

celeritas::connection_pool_fixture::guard_container_awaitable celeritas::connection_pool_fixture::get_many_sessions(const pool_shared_ptr& pool, const int count)
{
    guard_container guards{};
    guards.reserve(count);
    for (auto i = 0; i < count; ++i)
    {
        guards.emplace_back(co_await check_get_session(pool));
    }

    co_return guards;
}

celeritas::connection_pool_fixture::void_awaitable celeritas::connection_pool_fixture::async_wait(milliseconds_type duration)
{
    boost::asio::steady_timer timer{ co_await boost::asio::this_coro::executor };
    timer.expires_after(duration);
    co_await timer.async_wait(boost::asio::use_awaitable);
}

celeritas::connection_pool_fixture::void_awaitable celeritas::connection_pool_fixture::check_wait_get_session(const pool_shared_ptr& pool, atomic_bool_shared_ptr& waiter_finished)
{
    auto holder_guard = co_await check_get_session(pool);

    spawn([waiter_finished = waiter_finished,pool = pool]() -> boost::asio::awaitable<void> {
        auto waiter_guard = co_await check_get_session(pool);
        waiter_finished->store(true);
    });

    co_await async_wait(std::chrono::milliseconds{ 100 });

    BOOST_TEST(waiter_finished->load() == false);
}

void celeritas::connection_pool_fixture::set_test_end(const bool test_end)
{
    this->test_end_ = test_end;
}

celeritas::connection_pool_fixture::void_awaitable celeritas::connection_pool_fixture::test_routine(const pool_shared_ptr& pool)
{
    auto routine = [&]() -> boost::asio::awaitable<void> {
        for (auto i = 0; i < 10; ++i)
        {
            auto session = co_await pool->async_get_session();
            co_await async_wait(std::chrono::milliseconds{ 10 });
        }
    };

    std::vector<boost::asio::awaitable<void> > tasks{};
    for (auto i = 0; i < 5; ++i)
    {
        tasks.emplace_back(routine());
    }

    for (auto& task : tasks)
    {
        co_await std::move(task);
    }
}
