#include "fixture/database_session_test_fixture.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_session_suite)

    BOOST_FIXTURE_TEST_CASE(test_heartbeat_and_expiration, celeritas::database_session_test_fixture)
    {
        get_session().set_last_heartbeat();
        BOOST_CHECK(!get_session().is_expired());
    }

    BOOST_FIXTURE_TEST_CASE(test_is_health, celeritas::database_session_test_fixture)
    {
        run([this]() -> boost::asio::awaitable<void> {
            BOOST_CHECK(co_await get_session().is_health());
            BOOST_CHECK(get_session().get_is_health_called());

            set_test_end(true);
        });
    }

    BOOST_FIXTURE_TEST_CASE(test_execute_changes, celeritas::database_session_test_fixture)
    {
        run([this]() -> boost::asio::awaitable<void> {
            co_await get_session().execute_changes(get_update_change(), 0);
            BOOST_CHECK(get_session().get_execute_changes_called());

            set_test_end(true);
        });
    }

    BOOST_FIXTURE_TEST_CASE(test_select_one, celeritas::database_session_test_fixture)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto result = co_await get_session().select_one(get_select_change(), get_fields());
            BOOST_CHECK(!result.has_value());
            BOOST_CHECK(get_session().get_select_one_called());

            set_test_end(true);
        });
    }

    BOOST_FIXTURE_TEST_CASE(test_select_all, celeritas::database_session_test_fixture)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto result = co_await get_session().select_all(get_select_change(), get_fields());
            BOOST_CHECK(result.empty());
            BOOST_CHECK(get_session().get_select_all_called());

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
