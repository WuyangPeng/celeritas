#include "database/pool/connection_pool.tpp"
#include "database/pool/database_session_guard.tpp"
#include "mock/mock_pool_database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_session_guard_suite)

    BOOST_AUTO_TEST_CASE(test_move_constructor)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  auto guard1 = co_await pool->async_get_session();
                                  auto session1 = guard1.get_session();
                                  BOOST_TEST(session1 != nullptr);

                                  auto guard2{ std::move(guard1) };
                                  BOOST_TEST(guard1.get_session() == nullptr);
                                  BOOST_TEST(guard2.get_session() == session1);
                              },
                              boost::asio::detached);
        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_move_assignment)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 2);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  auto guard1 = co_await pool->async_get_session();
                                  auto session1 = guard1.get_session();
                                  BOOST_TEST(session1 != nullptr);

                                  auto guard2 = co_await pool->async_get_session();
                                  auto session2 = guard2.get_session();
                                  BOOST_TEST(session2 != nullptr);

                                  guard2 = std::move(guard1);
                                  BOOST_TEST(guard1.get_session() == nullptr);
                                  BOOST_TEST(guard2.get_session() == session1);
                              },
                              boost::asio::detached);
        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_get_session)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  auto guard = co_await pool->async_get_session();
                                  BOOST_TEST(guard.get_session() != nullptr);
                              },
                              boost::asio::detached);
        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_destructor)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  {
                                      auto guard = co_await pool->async_get_session();
                                      BOOST_TEST(guard.get_session() != nullptr);
                                  }

                                  auto guard2 = co_await pool->async_get_session();
                                  BOOST_TEST(guard2.get_session() != nullptr);
                              },
                              boost::asio::detached);
        io_context.run();
    }

BOOST_AUTO_TEST_SUITE_END()
