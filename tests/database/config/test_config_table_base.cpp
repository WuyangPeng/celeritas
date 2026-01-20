#include "fixture/config_manager_fixture.h"
#include "mock/mock_config_table.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(config_table_base_suite, celeritas::config_manager_fixture)

    BOOST_AUTO_TEST_CASE(test_get_name)
    {
        const celeritas::mock_config_table mock_table{};

        BOOST_CHECK_EQUAL(mock_table.get_name(), "mock_table");
    }

    BOOST_AUTO_TEST_CASE(test_load_all)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::mock_config_table mock_table{};

            BOOST_CHECK(!mock_table.was_load_all_called());
            co_await mock_table.load_all(nullptr);
            BOOST_CHECK(mock_table.was_load_all_called());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_load_one)
    {
        run([this]() -> boost::asio::awaitable<void> {
            celeritas::mock_config_table mock_table{};

            BOOST_CHECK(!mock_table.was_load_one_called());
            co_await mock_table.load_one(nullptr, 123);
            BOOST_CHECK(mock_table.was_load_one_called());

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
