#include "database/document/red_dots.h"
#include "config/luban/generated/schema.h"
#include "database/basic/basis_database.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(red_dots_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::red_dots data;
        BOOST_CHECK_EQUAL(data.is_state(), false);
        BOOST_CHECK_EQUAL(data.get_last_value(), 0);
        BOOST_CHECK_EQUAL(data.get_update_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::red_dots data;
        data.set_state(true);
        data.set_last_value(100);
        data.set_update_time(99999);

        // 假设 red_dot_type 是枚举
        auto node_id = static_cast<celeritas::config::red_dot_type>(1);
        data.set_node_id(node_id);

        BOOST_CHECK_EQUAL(data.is_state(), true);
        BOOST_CHECK_EQUAL(data.get_last_value(), 100);
        BOOST_CHECK_EQUAL(data.get_update_time(), 99999);
        BOOST_CHECK(data.get_node_id() == node_id);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::red_dots original;
        original.set_state(true);
        original.set_last_value(555);
        original.set_node_id(static_cast<celeritas::config::red_dot_type>(10));

        auto doc = original.to_document_type();
        auto restored = celeritas::red_dots::from_document(doc);

        BOOST_CHECK_EQUAL(restored.is_state(), true);
        BOOST_CHECK_EQUAL(restored.get_last_value(), 555);
        BOOST_CHECK(restored.get_node_id() == static_cast<celeritas::config::red_dot_type>(10));
    }

BOOST_AUTO_TEST_SUITE_END()