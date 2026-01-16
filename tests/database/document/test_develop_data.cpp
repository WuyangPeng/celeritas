#include "common/core/time_helper.h"
#include "database/basic/basis_database.h"
#include "database/document/develop_data.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(develop_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        constexpr celeritas::develop_data data{};

        BOOST_CHECK_EQUAL(data.get_system_id(), 0);
        BOOST_CHECK_EQUAL(data.get_instance_id(), 0);
        BOOST_CHECK_EQUAL(data.get_level(), celeritas::develop_data::default_level);
        BOOST_CHECK_EQUAL(data.get_exp(), 0);
        BOOST_CHECK_EQUAL(data.get_updated_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        constexpr auto system_id = 101;
        constexpr int64_t instance_id{ 202 };
        const celeritas::develop_data data{ system_id, instance_id };

        BOOST_CHECK_EQUAL(data.get_system_id(), system_id);
        BOOST_CHECK_EQUAL(data.get_instance_id(), instance_id);

        BOOST_CHECK_EQUAL(data.get_level(), celeritas::develop_data::default_level);
        BOOST_CHECK_EQUAL(data.get_exp(), 0);
        BOOST_CHECK_LE(data.get_updated_time(), celeritas::time_helper::get_current_milliseconds());
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::develop_data data{};

        data.set_system_id(1);
        BOOST_CHECK_EQUAL(data.get_system_id(), 1);

        data.set_instance_id(2);
        BOOST_CHECK_EQUAL(data.get_instance_id(), 2);

        data.set_level(10);
        BOOST_CHECK_EQUAL(data.get_level(), 10);

        data.set_exp(1000);
        BOOST_CHECK_EQUAL(data.get_exp(), 1000);

        data.set_updated_time(1234567890);
        BOOST_CHECK_EQUAL(data.get_updated_time(), 1234567890);
    }

    BOOST_AUTO_TEST_CASE(test_add_level)
    {
        celeritas::develop_data data{};

        data.set_level(5);
        data.add_level();
        BOOST_CHECK_EQUAL(data.get_level(), 6);
    }

    BOOST_AUTO_TEST_CASE(test_clear)
    {
        celeritas::develop_data data{ 1, 2 };
        data.set_level(10);
        data.set_exp(100);
        data.set_updated_time(500);

        data.clear();

        BOOST_CHECK_EQUAL(data.get_system_id(), 1);
        BOOST_CHECK_EQUAL(data.get_instance_id(), 2);
        BOOST_CHECK_EQUAL(data.get_level(), celeritas::develop_data::default_level);
        BOOST_CHECK_EQUAL(data.get_exp(), 0);
        BOOST_CHECK_LE(data.get_updated_time(), celeritas::time_helper::get_current_milliseconds());
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::develop_data original{ 10, 200 };
        original.set_level(5);
        original.set_exp(5000);
        original.set_updated_time(99999);

        const auto document = original.to_document_type();
        const auto restored = celeritas::develop_data::from_document(document);

        BOOST_CHECK_EQUAL(restored.get_system_id(), original.get_system_id());
        BOOST_CHECK_EQUAL(restored.get_instance_id(), original.get_instance_id());
        BOOST_CHECK_EQUAL(restored.get_level(), original.get_level());
        BOOST_CHECK_EQUAL(restored.get_exp(), original.get_exp());
        BOOST_CHECK_EQUAL(restored.get_updated_time(), original.get_updated_time());
    }

    BOOST_AUTO_TEST_CASE(test_from_document_partial)
    {
        const celeritas::develop_data::document_type document{ celeritas::basis_database{ celeritas::develop_data::system_id_description, 99 } };

        const auto data = celeritas::develop_data::from_document(document);

        BOOST_CHECK_EQUAL(data.get_system_id(), 99);
        BOOST_CHECK_EQUAL(data.get_instance_id(), 0);
    }

BOOST_AUTO_TEST_SUITE_END()