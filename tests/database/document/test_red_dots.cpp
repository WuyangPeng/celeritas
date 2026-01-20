#include "common/core/enum_cast.h"
#include "database/basic/basis_database.tpp"
#include "database/document/red_dots.h"
#include "common/core/time_helper.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(red_dots_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::red_dots data{};
        BOOST_CHECK(data.get_node_id() == celeritas::config::red_dot_type{});
        BOOST_CHECK_EQUAL(data.is_state(), false);
        BOOST_CHECK_EQUAL(data.get_last_value(), 0);
        BOOST_CHECK_EQUAL(data.get_update_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        const auto before = celeritas::time_helper::get_current_milliseconds();
        const celeritas::red_dots data{ celeritas::config::red_dot_type::role, true };
        const auto after = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK(data.get_node_id() == celeritas::config::red_dot_type::role);
        BOOST_CHECK_EQUAL(data.is_state(), true);
        BOOST_CHECK_EQUAL(data.get_last_value(), 0);
        BOOST_CHECK(data.get_update_time() >= before);
        BOOST_CHECK(data.get_update_time() <= after);
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
        original.set_update_time(123456789);
        original.set_node_id(celeritas::config::red_dot_type::role);

        const auto document = original.to_document_type();
        const auto restored = celeritas::red_dots::from_document(document);

        BOOST_CHECK_EQUAL(restored.is_state(), true);
        BOOST_CHECK_EQUAL(restored.get_last_value(), 555);
        BOOST_CHECK_EQUAL(restored.get_update_time(), 123456789);
        BOOST_CHECK(restored.get_node_id() == celeritas::config::red_dot_type::role);
    }

    BOOST_AUTO_TEST_CASE(test_from_document_with_missing_fields)
    {
        celeritas::red_dots::document_type partial_document;
        partial_document.emplace_back(celeritas::red_dots::node_id_description,
                                      celeritas::enum_cast_underlying(celeritas::config::red_dot_type::role));
        partial_document.emplace_back(celeritas::red_dots::state_description, true);

        const auto restored_from_partial = celeritas::red_dots::from_document(partial_document);
        BOOST_CHECK(restored_from_partial.get_node_id() == celeritas::config::red_dot_type::role);
        BOOST_CHECK_EQUAL(restored_from_partial.is_state(), true);
        BOOST_CHECK_EQUAL(restored_from_partial.get_last_value(), 0);
        BOOST_CHECK_EQUAL(restored_from_partial.get_update_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_from_document_with_extra_fields)
    {
        celeritas::red_dots::document_type extra_fields_document;
        extra_fields_document.emplace_back(celeritas::red_dots::node_id_description,
                                           celeritas::enum_cast_underlying(celeritas::config::red_dot_type::role));
        extra_fields_document.emplace_back("extra_field", "some_value");

        const auto restored_from_extra = celeritas::red_dots::from_document(extra_fields_document);
        BOOST_CHECK(restored_from_extra.get_node_id() == celeritas::config::red_dot_type::role);
        BOOST_CHECK_EQUAL(restored_from_extra.is_state(), false);
    }

BOOST_AUTO_TEST_SUITE_END()
