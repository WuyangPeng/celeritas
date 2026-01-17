#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_field.h"
#include "database/pool/connection_pool.tpp"
#include "fixture/connection_pool_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(connection_pool_suite, celeritas::connection_pool_fixture)

    BOOST_AUTO_TEST_CASE(test_connection_pool_with_mock_session)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5);

            co_await pool->async_initialize();

            co_await check_get_session(pool);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_connection_pool_with_uri_constructor)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "mongodb://localhost:27017",
                                                          "test_db",
                                                          1,
                                                          5);

            co_await pool->async_initialize();

            co_await check_get_session(pool);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_is_health)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5);

            co_await pool->async_initialize();

            BOOST_TEST(co_await pool->is_health());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_execute_changes)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5);

            co_await pool->async_initialize();

            const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
            const auto dummy_change = std::make_shared<celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                          "test_db",
                                                                                          celeritas::database_change_type::update_type,
                                                                                          key_container);
            BOOST_TEST(co_await pool->execute_changes(dummy_change, 0));

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_select_one)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5);

            co_await pool->async_initialize();

            const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
            const auto dummy_change = std::make_shared<celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                          "test_db",
                                                                                          celeritas::database_change_type::update_type,
                                                                                          key_container);
            const auto result_one = co_await pool->select_one(dummy_change, {});
            BOOST_TEST(!result_one.has_value());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_select_all)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5);
            co_await pool->async_initialize();

            const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
            const auto dummy_change = std::make_shared<celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                          "test_db",
                                                                                          celeritas::database_change_type::update_type,
                                                                                          key_container);
            const auto result_all = co_await pool->select_all(dummy_change, {});
            BOOST_TEST(result_all.empty());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_cleanup_database_by_duration)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          5,
                                                          0);
            co_await pool->async_initialize();

            auto guards = co_await get_many_sessions(pool, 5);

            guards.clear();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            pool->cleanup_database_by_duration();

            co_await check_get_session(pool);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_pool_full_and_wait)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<pool_type>(get_io_context().get_executor(),
                                                          "localhost",
                                                          3306,
                                                          "user",
                                                          "password",
                                                          "test_db",
                                                          1,
                                                          1);

            auto waiter_finished = std::make_shared<std::atomic_bool>(false);

            co_await pool->async_initialize();

            co_await check_wait_get_session(pool, waiter_finished);

            co_await async_wait(std::chrono::milliseconds{ 100 });

            BOOST_TEST(waiter_finished->load() == true);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
