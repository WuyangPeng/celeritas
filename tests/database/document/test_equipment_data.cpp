#include "database/basic/basis_database.tpp"
#include "database/document/equipment_data.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(equipment_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::equipment_data data{};

        BOOST_CHECK_EQUAL(data.get_strength(), 0);
        BOOST_CHECK_EQUAL(data.get_durability(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        const celeritas::equipment_data data{ 10, 100 };

        BOOST_CHECK_EQUAL(data.get_strength(), 10);
        BOOST_CHECK_EQUAL(data.get_durability(), 100);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::equipment_data data{};

        data.set_strength(5);
        data.set_durability(50);
        BOOST_CHECK_EQUAL(data.get_strength(), 5);
        BOOST_CHECK_EQUAL(data.get_durability(), 50);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        const celeritas::equipment_data original{ 20, 200 };

        const auto document = original.to_document_type();
        const auto restored = celeritas::equipment_data::from_document(document);

        BOOST_CHECK_EQUAL(restored.get_strength(), original.get_strength());
        BOOST_CHECK_EQUAL(restored.get_durability(), original.get_durability());
    }

    BOOST_AUTO_TEST_CASE(test_from_document_partial)
    {
        const celeritas::equipment_data::document_type document{ celeritas::basis_database{ celeritas::equipment_data::strength_description, 99 } };

        const auto data = celeritas::equipment_data::from_document(document);

        BOOST_CHECK_EQUAL(data.get_strength(), 99);
        BOOST_CHECK_EQUAL(data.get_durability(), 0);
    }

BOOST_AUTO_TEST_SUITE_END()