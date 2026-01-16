#include "database/basic/basis_database.tpp"
#include "database/document/red_dots.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(red_dots_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::red_dots data{};
        BOOST_CHECK_EQUAL(data.is_state(), false);
        BOOST_CHECK_EQUAL(data.get_last_value(), 0);
        BOOST_CHECK_EQUAL(data.get_update_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::red_dots data{};

        data.set_state(true);
        data.set_last_value(100);
        data.set_update_time(99999);

        constexpr auto node_id = celeritas::config::red_dot_type::role;
        data.set_node_id(node_id);

        BOOST_CHECK_EQUAL(data.is_state(), true);
        BOOST_CHECK_EQUAL(data.get_last_value(), 100);
        BOOST_CHECK_EQUAL(data.get_update_time(), 99999);
        BOOST_CHECK(data.get_node_id() == node_id);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::red_dots original{};

        original.set_state(true);
        original.set_last_value(555);
        original.set_node_id(celeritas::config::red_dot_type::role);

        const auto document = original.to_document_type();
        const auto restored = celeritas::red_dots::from_document(document);

        BOOST_CHECK_EQUAL(restored.is_state(), true);
        BOOST_CHECK_EQUAL(restored.get_last_value(), 555);
        BOOST_CHECK(restored.get_node_id() == celeritas::config::red_dot_type::role);
    }

BOOST_AUTO_TEST_SUITE_END()