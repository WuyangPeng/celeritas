#include "database/pool/connection_pool.tpp"
#include "database/pool/database_session_guard.tpp"
#include "fixture/connection_pool_fixture.h"
#include "mock/mock_pool_database_session.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(database_session_guard_suite, celeritas::connection_pool_fixture)

    BOOST_AUTO_TEST_CASE(test_move_constructor)
    {
        auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(get_io_context().get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        run([this,pool = std::move(pool)]() -> boost::asio::awaitable<void> {
            co_await pool->async_initialize();

            auto guard1 = co_await check_get_session(pool);
            auto session = guard1.get_session();

            auto guard2{ std::move(guard1) };
            BOOST_TEST(guard1.get_session() == nullptr);
            BOOST_TEST(guard2.get_session() == session);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_move_assignment)
    {
        auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(get_io_context().get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 2);

        run([this,pool = std::move(pool)]() -> boost::asio::awaitable<void> {
            co_await pool->async_initialize();

            auto guard1 = co_await check_get_session(pool);
            auto session = guard1.get_session();

            auto guard2 = co_await check_get_session(pool);

            guard2 = std::move(guard1);
            BOOST_TEST(guard1.get_session() == nullptr);
            BOOST_TEST(guard2.get_session() == session);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_session)
    {
        auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(get_io_context().get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        run([this,pool= std::move(pool)]() -> boost::asio::awaitable<void> {
            co_await pool->async_initialize();

            co_await check_get_session(pool);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_destructor)
    {
        auto pool = std::make_shared<celeritas::connection_pool<celeritas::mock_pool_database_session> >(get_io_context().get_executor(), "localhost", 3306, "user", "password", "test_db", 1, 1);

        run([this,pool = std::move(pool)]() -> boost::asio::awaitable<void> {
            co_await pool->async_initialize();

            co_await check_get_session(pool);
            co_await check_get_session(pool);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
