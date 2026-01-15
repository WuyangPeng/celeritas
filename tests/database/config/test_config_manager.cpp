#include "database/config/config_manager.h"
#include "database/pool/database_pool_manager.h"
#include "fixture/config_manager_fixture.h"
#include "mock/mock_config_database_pool.h"

#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_SUITE_END()
