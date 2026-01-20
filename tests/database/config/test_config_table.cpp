#include "database/config/config_table.h"
#include "database/generated/mysql/config/time_refresh.h"
#include "database/pool/database_pool_manager.h"
#include "fixture/config_manager_fixture.h"
#include "mock/mock_config_database_pool.h"

#include <boost/test/unit_test.hpp>

namespace
{
    void check_item_valid(const std::optional<std::shared_ptr<const celeritas::time_refresh>>& item)
    {
        BOOST_CHECK(item.has_value());
        BOOST_CHECK_EQUAL(item.value()->get_id(), celeritas::mock_config_database_pool::time_refresh_id);
        BOOST_CHECK_EQUAL(item.value()->get_time_refresh_type(), 2);
        BOOST_CHECK_EQUAL(item.value()->get_parameter(), 3);
    }
}

BOOST_FIXTURE_TEST_SUITE(config_table_suite, celeritas::config_manager_fixture)

    BOOST_AUTO_TEST_CASE(test_get_name)
    {
        const std::string table_name{ "test_table" };
        const celeritas::config_table<celeritas::time_refresh> table{ table_name };
        BOOST_CHECK_EQUAL(table.get_name(), table_name);
    }

    BOOST_AUTO_TEST_CASE(test_load_all)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto pool = celeritas::database_pool_manager::get_instance().get_pool(celeritas::mysql_config_db_name.data());
            BOOST_REQUIRE(pool);

            celeritas::config_table<celeritas::time_refresh> table{ celeritas::time_refresh_db_name.data() };
            co_await table.load_all(pool);

            const auto item = table.get_item(celeritas::mock_config_database_pool::time_refresh_id);
            check_item_valid(item);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_load_one)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto pool = celeritas::database_pool_manager::get_instance().get_pool(celeritas::mysql_config_db_name.data());
            BOOST_REQUIRE(pool);

            celeritas::config_table<celeritas::time_refresh> table{ celeritas::time_refresh_db_name.data() };
            co_await table.load_one(pool, celeritas::mock_config_database_pool::time_refresh_id);

            const auto item = table.get_item(celeritas::mock_config_database_pool::time_refresh_id);
            check_item_valid(item);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_non_existent_item)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto pool = celeritas::database_pool_manager::get_instance().get_pool(celeritas::mysql_config_db_name.data());
            BOOST_REQUIRE(pool);

            celeritas::config_table<celeritas::time_refresh> table{ celeritas::time_refresh_db_name.data() };
            co_await table.load_all(pool);

            const auto non_existent_item = table.get_item(0);
            BOOST_CHECK(!non_existent_item.has_value());

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
