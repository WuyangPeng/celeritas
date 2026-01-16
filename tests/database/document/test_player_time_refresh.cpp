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

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::player_time_refresh original{};

        original.set_parameter(100);
        original.set_time_id(1);
        original.set_last_refresh_time(123456789);
        original.add_component(celeritas::player_component_type::attribute);
        original.set_time_refresh_type(celeritas::time_refresh_type::daily);

        const auto document = original.to_document_type();
        const auto restored = celeritas::player_time_refresh::from_document(document);

        BOOST_CHECK_EQUAL(restored.get_parameter(), 100);
        BOOST_CHECK_EQUAL(restored.get_time_id(), 1);
        BOOST_CHECK_EQUAL(restored.get_last_refresh_time(), 123456789);
        BOOST_CHECK_EQUAL(restored.get_component().size(), 1);
        BOOST_CHECK(restored.get_time_refresh_type() == celeritas::time_refresh_type::daily);
    }

BOOST_AUTO_TEST_SUITE_END()