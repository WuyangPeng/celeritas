#include "database/document/server_role.h"
#include "database/basic/basis_database.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(server_role_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::server_role data;
        BOOST_CHECK(data.get_game_server_id().empty());
        BOOST_CHECK(data.get_role_surname().empty());
        BOOST_CHECK(data.get_role_name().empty());
        BOOST_CHECK_EQUAL(data.get_last_login_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        celeritas::server_role data("server1", "Doe", "John");
        BOOST_CHECK_EQUAL(data.get_game_server_id(), "server1");
        BOOST_CHECK_EQUAL(data.get_role_surname(), "Doe");
        BOOST_CHECK_EQUAL(data.get_role_name(), "John");
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::server_role original("s1", "Smith", "Agent");
        original.set_last_login_time(123456789);

        auto doc = original.to_document_type();
        auto restored = celeritas::server_role::from_document(doc);

        BOOST_CHECK_EQUAL(restored.get_game_server_id(), "s1");
        BOOST_CHECK_EQUAL(restored.get_role_surname(), "Smith");
        BOOST_CHECK_EQUAL(restored.get_role_name(), "Agent");
        BOOST_CHECK_EQUAL(restored.get_last_login_time(), 123456789);
    }

BOOST_AUTO_TEST_SUITE_END()