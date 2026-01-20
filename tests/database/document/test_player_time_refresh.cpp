#include "common/core/time_helper.h"
#include "database/basic/basis_database.tpp"
#include "database/document/player_time_refresh.h"
#include "player/component/player_component_type.h"
#include "player/time/time_refresh_type.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(player_time_refresh_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::player_time_refresh data{};

        BOOST_CHECK_EQUAL(data.get_parameter(), 0);
        BOOST_CHECK_EQUAL(data.get_time_id(), 0);
        BOOST_CHECK_EQUAL(data.get_last_refresh_time(), 0);
        BOOST_CHECK(data.get_component().empty());
        BOOST_CHECK(data.get_time_refresh_type() == celeritas::time_refresh_type{});
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        constexpr auto component = celeritas::player_component_type::role;
        const celeritas::player_time_refresh data{ celeritas::time_refresh_type::daily, 100, 1, component };

        BOOST_CHECK(data.get_time_refresh_type() == celeritas::time_refresh_type::daily);
        BOOST_CHECK_EQUAL(data.get_parameter(), 100);
        BOOST_CHECK_EQUAL(data.get_time_id(), 1);
        BOOST_CHECK_EQUAL(data.get_component().size(), 1);
        BOOST_CHECK(data.get_component().at(0) == component);
        BOOST_CHECK_GT(data.get_last_refresh_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_component_operations)
    {
        celeritas::player_time_refresh data{};

        constexpr auto component1 = celeritas::player_component_type::role;
        constexpr auto component2 = celeritas::player_component_type::online;

        data.add_component(component1);
        BOOST_CHECK_EQUAL(data.get_component().size(), 1);

        data.add_component(component2);
        BOOST_CHECK_EQUAL(data.get_component().size(), 2);

        data.remove_component(component1);
        BOOST_CHECK_EQUAL(data.get_component().size(), 1);
        BOOST_CHECK(data.get_component().at(0) == component2);
    }

    BOOST_AUTO_TEST_CASE(test_is_default_daily)
    {
        celeritas::player_time_refresh data{};
        data.set_time_refresh_type(celeritas::time_refresh_type::daily);

        data.set_parameter(3600000);
        BOOST_CHECK(data.is_default());

        data.set_parameter(7200000);
        BOOST_CHECK(data.is_default());

        data.set_parameter(3600001);
        BOOST_CHECK(!data.is_default());
    }

    BOOST_AUTO_TEST_CASE(test_is_default_weekly)
    {
        celeritas::player_time_refresh data{};
        data.set_time_refresh_type(celeritas::time_refresh_type::weekly);

        data.set_parameter(0);
        BOOST_CHECK(data.is_default());

        data.set_parameter(86400000);
        BOOST_CHECK(data.is_default());

        data.set_parameter(86400001);
        BOOST_CHECK(!data.is_default());
    }

    BOOST_AUTO_TEST_CASE(test_is_default_monthly)
    {
        celeritas::player_time_refresh data{};
        data.set_time_refresh_type(celeritas::time_refresh_type::monthly);

        data.set_parameter(0);
        BOOST_CHECK(data.is_default());

        data.set_parameter(1);
        BOOST_CHECK(!data.is_default());
    }

    BOOST_AUTO_TEST_CASE(test_is_default_others)
    {
        celeritas::player_time_refresh data{};

        data.set_time_refresh_type(celeritas::time_refresh_type::interval_duration);
        BOOST_CHECK(!data.is_default());

        data.set_time_refresh_type(celeritas::time_refresh_type::absolute_point);
        BOOST_CHECK(!data.is_default());
    }

    BOOST_AUTO_TEST_CASE(test_refresh_logic_interval)
    {
        celeritas::player_time_refresh data{};
        const auto now = celeritas::time_helper::get_current_milliseconds();
        data.set_time_refresh_type(celeritas::time_refresh_type::interval_duration);
        data.set_parameter(1000);

        data.set_last_refresh_time(now - 2000);
        BOOST_CHECK(data.is_can_refresh());

        data.set_last_refresh_time(now - 500);
        BOOST_CHECK(!data.is_can_refresh());
    }

    BOOST_AUTO_TEST_CASE(test_refresh_logic_absolute)
    {
        celeritas::player_time_refresh data{};
        const auto now = celeritas::time_helper::get_current_milliseconds();
        data.set_time_refresh_type(celeritas::time_refresh_type::absolute_point);

        data.set_parameter(now - 1000);
        BOOST_CHECK(data.is_can_refresh());

        data.set_parameter(now + 1000);
        BOOST_CHECK(!data.is_can_refresh());
    }

    BOOST_AUTO_TEST_CASE(test_refresh_logic_daily)
    {
        celeritas::player_time_refresh data{};
        const auto now = celeritas::time_helper::get_current_milliseconds();
        data.set_time_refresh_type(celeritas::time_refresh_type::daily);
        data.set_parameter(0);
        data.set_last_refresh_time(now - 86400000 * 2);
        BOOST_CHECK(data.is_can_refresh());
        BOOST_CHECK_GT(data.get_next_refresh_time(), now);
    }

    BOOST_AUTO_TEST_CASE(test_refresh_logic_weekly)
    {
        celeritas::player_time_refresh data{};
        const auto now = celeritas::time_helper::get_current_milliseconds();
        data.set_time_refresh_type(celeritas::time_refresh_type::weekly);
        data.set_last_refresh_time(now - 86400000 * 8);
        BOOST_CHECK(data.is_can_refresh());
        BOOST_CHECK_GT(data.get_next_refresh_time(), now);
    }

    BOOST_AUTO_TEST_CASE(test_refresh_logic_monthly)
    {
        celeritas::player_time_refresh data{};
        const auto now = celeritas::time_helper::get_current_milliseconds();
        data.set_time_refresh_type(celeritas::time_refresh_type::monthly);
        data.set_last_refresh_time(now - 86400000LL * 32);
        BOOST_CHECK(data.is_can_refresh());
        BOOST_CHECK_GT(data.get_next_refresh_time(), now);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::player_time_refresh original{};
        original.set_time_refresh_type(celeritas::time_refresh_type::daily);
        original.set_parameter(100);
        original.set_time_id(1);
        original.set_last_refresh_time(123456789);
        original.add_component(celeritas::player_component_type::attribute);

        const auto document = original.to_document_type();
        const auto restored = celeritas::player_time_refresh::from_document(document);

        BOOST_CHECK(restored.get_time_refresh_type() == original.get_time_refresh_type());
        BOOST_CHECK_EQUAL(restored.get_parameter(), original.get_parameter());
        BOOST_CHECK_EQUAL(restored.get_time_id(), original.get_time_id());
        BOOST_CHECK_EQUAL(restored.get_last_refresh_time(), original.get_last_refresh_time());
        BOOST_CHECK_EQUAL(restored.get_component().size(), original.get_component().size());
        BOOST_CHECK(restored.get_component().at(0) == original.get_component().at(0));
    }

BOOST_AUTO_TEST_SUITE_END()