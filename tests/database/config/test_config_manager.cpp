#include "database/config/config_manager.h"
#include "database/pool/database_pool_manager.h"
#include "detail/concurrent_access_tester.h"
#include "fixture/config_manager_fixture.h"
#include "mock/mock_config_database_pool.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(config_manager_suite, celeritas::config_manager_fixture)

    BOOST_AUTO_TEST_CASE(test_singleton_instance)
    {
        const auto& instance1 = celeritas::config_manager::get_instance();
        const auto& instance2 = celeritas::config_manager::get_instance();

        BOOST_CHECK(&instance1 == &instance2);

        set_test_end(true);
    }

    BOOST_AUTO_TEST_CASE(test_load_and_get_time_refresh)
    {
        run([this]() -> boost::asio::awaitable<void> {
            auto& config_manager = celeritas::config_manager::get_instance();
            config_manager.load_from_db(get_io_context().get_executor());

            co_await boost::asio::steady_timer(get_io_context().get_executor(), std::chrono::milliseconds{ 100 }).async_wait(boost::asio::use_awaitable);

            check_time_refresh_valid();

            const auto non_existent = config_manager.get_time_refresh(999);
            BOOST_CHECK(!non_existent.has_value());
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_reload_from_db_with_params)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::config_manager::get_instance().reload_from_db(get_io_context().get_executor(), "time_refresh_db", celeritas::mock_config_database_pool::time_refresh_id);

            co_await boost::asio::steady_timer(get_io_context().get_executor(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            check_time_refresh_valid();

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_reload_from_db_empty_params)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::config_manager::get_instance().reload_from_db(get_io_context().get_executor(), "", 0);

            co_await boost::asio::steady_timer(get_io_context().get_executor(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_reload_from_db_with_empty_db_name_and_non_zero_id)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::config_manager::get_instance().reload_from_db(get_io_context().get_executor(), "", celeritas::mock_config_database_pool::time_refresh_id);

            co_await boost::asio::steady_timer(get_io_context().get_executor(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            check_time_refresh_valid();

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_concurrent_access)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::config_manager::get_instance().load_from_db(get_io_context().get_executor());

            co_await boost::asio::steady_timer(get_io_context().get_executor(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            celeritas::concurrent_access_tester tester{ get_io_context().get_executor() };
            tester.run();
            co_await tester.wait();

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
