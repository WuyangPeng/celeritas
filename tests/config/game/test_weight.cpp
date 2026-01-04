#include "common/core/celeritas_error.h"
#include "config/game/weight.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(weight_suite)

    BOOST_AUTO_TEST_CASE(test_weight_add_and_get)
    {
        celeritas::weight weight{};

        weight.add_element(101, 0.5);
        weight.add_element(102, 1.5);

        const auto& weights = weight.get_weights();
        BOOST_CHECK_EQUAL(weights.size(), 2);
        BOOST_CHECK_CLOSE(weights[0], 0.5, 0.001);
        BOOST_CHECK_CLOSE(weights[1], 1.5, 0.001);

        BOOST_CHECK_EQUAL(weight.get_id(0), 101);
        BOOST_CHECK_EQUAL(weight.get_id(1), 102);
    }

    BOOST_AUTO_TEST_CASE(test_weight_clear)
    {
        celeritas::weight weight{};
        weight.add_element(1, 10.0);
        BOOST_CHECK_EQUAL(weight.get_weights().size(), 1);

        weight.clear();
        BOOST_CHECK(weight.get_weights().empty());
    }

    BOOST_AUTO_TEST_CASE(test_weight_get_id_out_of_range)
    {
        celeritas::weight weight{};
        weight.add_element(1, 10.0);

        BOOST_CHECK_EQUAL(weight.get_id(0), 1);
    }

BOOST_AUTO_TEST_SUITE_END()
