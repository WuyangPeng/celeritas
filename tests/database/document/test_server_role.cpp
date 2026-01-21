#include "database/basic/basis_database.tpp"
#include "database/document/server_role.h"
#include "common/core/time_helper.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(server_role_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::server_role data{};

        BOOST_CHECK(data.get_game_server_id().empty());
        BOOST_CHECK(data.get_role_surname().empty());
        BOOST_CHECK(data.get_role_name().empty());
        BOOST_CHECK_EQUAL(data.get_last_login_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        const auto before = celeritas::time_helper::get_current_milliseconds();
        const celeritas::server_role data{ "server1", "Doe", "John" };
        const auto after = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK_EQUAL(data.get_game_server_id(), "server1");
        BOOST_CHECK_EQUAL(data.get_role_surname(), "Doe");
        BOOST_CHECK_EQUAL(data.get_role_name(), "John");
        BOOST_CHECK(data.get_last_login_time() >= before);
        BOOST_CHECK(data.get_last_login_time() <= after);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::server_role data{};

        data.set_game_server_id("server2");
        data.set_role_surname("Smith");
        data.set_role_name("Jane");
        data.set_last_login_time(987654321);

        BOOST_CHECK_EQUAL(data.get_game_server_id(), "server2");
        BOOST_CHECK_EQUAL(data.get_role_surname(), "Smith");
        BOOST_CHECK_EQUAL(data.get_role_name(), "Jane");
        BOOST_CHECK_EQUAL(data.get_last_login_time(), 987654321);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::server_role original{ "s1", "Smith", "Agent" };
        original.set_last_login_time(123456789);

        const auto document = original.to_document_type();
        const auto restored = celeritas::server_role::from_document(document);

        BOOST_CHECK_EQUAL(restored.get_game_server_id(), "s1");
        BOOST_CHECK_EQUAL(restored.get_role_surname(), "Smith");
        BOOST_CHECK_EQUAL(restored.get_role_name(), "Agent");
        BOOST_CHECK_EQUAL(restored.get_last_login_time(), 123456789);
    }

    BOOST_AUTO_TEST_CASE(test_from_document_with_missing_fields)
    {
        celeritas::server_role::document_type partial_document{};
        partial_document.emplace_back(celeritas::server_role::game_server_id_description, "server3");
        partial_document.emplace_back(celeritas::server_role::role_name_description, "Neo");

        const auto restored = celeritas::server_role::from_document(partial_document);

        BOOST_CHECK_EQUAL(restored.get_game_server_id(), "server3");
        BOOST_CHECK(restored.get_role_surname().empty());
        BOOST_CHECK_EQUAL(restored.get_role_name(), "Neo");
        BOOST_CHECK_EQUAL(restored.get_last_login_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_from_document_with_extra_fields)
    {
        celeritas::server_role::document_type extra_fields_document{};
        extra_fields_document.emplace_back(celeritas::server_role::game_server_id_description, "server4");
        extra_fields_document.emplace_back("extra_field", "some_value");
        extra_fields_document.emplace_back(celeritas::server_role::role_name_description, "Trinity");

        const auto restored = celeritas::server_role::from_document(extra_fields_document);

        BOOST_CHECK_EQUAL(restored.get_game_server_id(), "server4");
        BOOST_CHECK(restored.get_role_surname().empty());
        BOOST_CHECK_EQUAL(restored.get_role_name(), "Trinity");
        BOOST_CHECK_EQUAL(restored.get_last_login_time(), 0);
    }

BOOST_AUTO_TEST_SUITE_END()
