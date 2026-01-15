#include "database/config/config_manager.h"
#include "database/pool/database_pool_manager.h"
#include "fixture/config_manager_fixture.h"
#include "mock/mock_config_database_pool.h"

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_FIXTURE_TEST_SUITE(config_manager_suite, celeritas::config_manager_fixture)

    BOOST_AUTO_TEST_CASE(test_load_and_get_time_refresh)
    {
        run([this]() -> boost::asio::awaitable<void> {
            auto& config_manager = celeritas::config_manager::get_instance();
            config_manager.load_from_db(get_io_context().get_executor());

            co_await boost::asio::steady_timer(get_io_context(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            const auto time_refresh_opt = config_manager.get_time_refresh(1);
            BOOST_REQUIRE(time_refresh_opt.has_value());
            BOOST_CHECK_EQUAL(time_refresh_opt->get_time_refresh_type(), 1);
            BOOST_CHECK_EQUAL(time_refresh_opt->get_parameter(), 2);

            const auto non_existent = config_manager.get_time_refresh(999);
            BOOST_CHECK(!non_existent.has_value());
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_singleton_instance)
    {
        const auto& instance1 = celeritas::config_manager::get_instance();
        const auto& instance2 = celeritas::config_manager::get_instance();

        BOOST_CHECK(&instance1 == &instance2);

        set_test_end(true);
    }

    BOOST_AUTO_TEST_CASE(test_reload_from_db_with_params)
    {
        run([this]() -> boost::asio::awaitable<void> {
            auto& config_manager = celeritas::config_manager::get_instance();

            config_manager.reload_from_db(get_io_context().get_executor(), "time_refresh_db", 1);

            co_await boost::asio::steady_timer(get_io_context(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            const auto time_refresh_opt = config_manager.get_time_refresh(1);
            BOOST_REQUIRE(time_refresh_opt.has_value());
            BOOST_CHECK_EQUAL(time_refresh_opt->get_time_refresh_type(), 1);
            BOOST_CHECK_EQUAL(time_refresh_opt->get_parameter(), 2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_reload_from_db_empty_params)
    {
        run([this]() -> boost::asio::awaitable<void> {
            auto& config_manager = celeritas::config_manager::get_instance();

            config_manager.reload_from_db(get_io_context().get_executor(), "", 0);

            co_await boost::asio::steady_timer(get_io_context(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_concurrent_access)
    {
        run([this]() -> boost::asio::awaitable<void> {
            auto& config_manager = celeritas::config_manager::get_instance();
            config_manager.load_from_db(get_io_context().get_executor());

            co_await boost::asio::steady_timer(get_io_context(), std::chrono::milliseconds(100)).async_wait(boost::asio::use_awaitable);

            std::vector<boost::asio::awaitable<void> > tasks{};
            for (auto i = 0; i < 10; ++i)
            {
                tasks.emplace_back([&config_manager]() -> boost::asio::awaitable<void> {
                    if (const auto time_refresh_opt = config_manager.get_time_refresh(1);
                        time_refresh_opt.has_value())
                    {
                        BOOST_CHECK_EQUAL(time_refresh_opt->get_time_refresh_type(), 1);
                    }
                    else
                    {
                        BOOST_CHECK(true);
                    }
                    co_return;
                }());
            }

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()