#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_field.h"
#include "database/pool/connection_pool.tpp"
#include "mock/mock_pool_database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(connection_pool_suite)

    BOOST_AUTO_TEST_CASE(test_connection_pool_with_mock_session)
    {
        boost::asio::io_context io_context{};

        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(),
                                                                                                               "localhost",
                                                                                                               3306,
                                                                                                               "user",
                                                                                                               "password",
                                                                                                               "test_db",
                                                                                                               1,
                                                                                                               5);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  auto session_guard = co_await pool->async_get_session();
                                  BOOST_TEST(session_guard.get_session() != nullptr);
                              },
                              boost::asio::detached);

        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_connection_pool_with_uri_constructor)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(),
                                                                                                               "mongodb://localhost:27017",
                                                                                                               "test_db",
                                                                                                               1,
                                                                                                               5);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();
                                  auto session_guard = co_await pool->async_get_session();
                                  BOOST_TEST(session_guard.get_session() != nullptr);
                              },
                              boost::asio::detached);

        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_connection_pool_public_interfaces)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(),
                                                                                                               "localhost",
                                                                                                               3306,
                                                                                                               "user",
                                                                                                               "password",
                                                                                                               "test_db",
                                                                                                               1,
                                                                                                               5);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();

                                  BOOST_TEST(co_await pool->is_health());

                                  const auto dummy_change = std::make_shared<celeritas::database_entity_change>(celeritas::database_type::mysql, "test_db", celeritas::database_change_type::update_type);
                                  BOOST_TEST(co_await pool->execute_changes(dummy_change, 0));

                                  const auto result_one = co_await pool->select_one(dummy_change, {});
                                  BOOST_TEST(!result_one.has_value());

                                  const auto result_all = co_await pool->select_all(dummy_change, {});
                                  BOOST_TEST(result_all.empty());
                              },
                              boost::asio::detached);

        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_cleanup_database_by_duration)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(),
                                                                                                               "localhost",
                                                                                                               3306,
                                                                                                               "user",
                                                                                                               "password",
                                                                                                               "test_db",
                                                                                                               1,
                                                                                                               5,
                                                                                                               0);

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();

                                  std::vector<celeritas::database_session_guard<celeritas::mock_pool_database_session> > guards{};
                                  for (auto i = 0; i < 5; ++i)
                                  {
                                      guards.emplace_back(co_await pool->async_get_session());
                                  }

                                  guards.clear();

                                  std::this_thread::sleep_for(std::chrono::seconds(1));

                                  pool->cleanup_database_by_duration();

                                  auto session_guard = co_await pool->async_get_session();
                                  BOOST_TEST(session_guard.get_session() != nullptr);
                              },
                              boost::asio::detached);

        io_context.run();
    }

    BOOST_AUTO_TEST_CASE(test_pool_full_and_wait)
    {
        boost::asio::io_context io_context{};

        const auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(io_context.get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        std::atomic_bool waiter_finished{ false };

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await pool->async_initialize();

                                  {
                                      auto holder_guard = co_await pool->async_get_session();
                                      BOOST_TEST(holder_guard.get_session() != nullptr);

                                      boost::asio::co_spawn(io_context, [&]() -> boost::asio::awaitable<void> {
                                          auto waiter_guard = co_await pool->async_get_session();
                                          BOOST_TEST(waiter_guard.get_session() != nullptr);
                                          waiter_finished = true;
                                      }, boost::asio::detached);

                                      boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor);
                                      timer.expires_after(std::chrono::milliseconds(100));
                                      co_await timer.async_wait(boost::asio::use_awaitable);

                                      BOOST_TEST(waiter_finished == false);
                                  }

                                  boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor);
                                  timer.expires_after(std::chrono::milliseconds(100));
                                  co_await timer.async_wait(boost::asio::use_awaitable);

                                  BOOST_TEST(waiter_finished == true);
                              },
                              boost::asio::detached);

        io_context.run();
    }

BOOST_AUTO_TEST_SUITE_END()
