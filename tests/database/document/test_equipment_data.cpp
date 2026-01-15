#include "database/document/equipment_data.h"
#include "database/basic/basis_database.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(equipment_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::equipment_data data;
        BOOST_CHECK_EQUAL(data.get_strength(), 0);
        BOOST_CHECK_EQUAL(data.get_durability(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        celeritas::equipment_data data(10, 100);
        BOOST_CHECK_EQUAL(data.get_strength(), 10);
        BOOST_CHECK_EQUAL(data.get_durability(), 100);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::equipment_data data;
        data.set_strength(5);
        data.set_durability(50);
        BOOST_CHECK_EQUAL(data.get_strength(), 5);
        BOOST_CHECK_EQUAL(data.get_durability(), 50);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::equipment_data original(20, 200);
        auto doc = original.to_document_type();
        auto restored = celeritas::equipment_data::from_document(doc);
        BOOST_CHECK_EQUAL(restored.get_strength(), 20);
        BOOST_CHECK_EQUAL(restored.get_durability(), 200);
    }

BOOST_AUTO_TEST_SUITE_END()