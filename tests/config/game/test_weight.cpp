#include "common/core/celeritas_error.h"
#include "config/game/weight.h"

#include <boost/test/unit_test.hpp>
#include <set>

BOOST_AUTO_TEST_SUITE(weight_suite)

    BOOST_AUTO_TEST_CASE(test_weight_add_and_get_id)
    {
        celeritas::weight weight{};

        weight.add_element(101, 0.5);
        weight.add_element(102, 1.5);

        BOOST_CHECK_EQUAL(weight.get_id(0), 101);
        BOOST_CHECK_EQUAL(weight.get_id(1), 102);
    }

    BOOST_AUTO_TEST_CASE(test_weight_clear)
    {
        celeritas::weight weight{};
        weight.add_element(1, 10.0);

        weight.clear();
        weight.add_element(201, 1.0);
        BOOST_CHECK_EQUAL(weight.get_id(0), 201);

        BOOST_CHECK_THROW(std::ignore = weight.get_id(1), std::out_of_range);
    }

    BOOST_AUTO_TEST_CASE(test_weight_get_id_out_of_range)
    {
        celeritas::weight weight{};
        weight.add_element(1, 10.0);

        BOOST_CHECK_EQUAL(weight.get_id(0), 1);
        BOOST_CHECK_THROW(std::ignore = weight.get_id(1), std::out_of_range);
    }

    BOOST_AUTO_TEST_CASE(test_get_random_index_by_weight)
    {
        celeritas::weight weight{};
        weight.add_element(101, 10.0);

        BOOST_CHECK_EQUAL(weight.get_random_index_by_weight(), 0);

        weight.add_element(102, 20.0);

        for (auto i = 0; i < 10; ++i)
        {
            const auto index = weight.get_random_index_by_weight();
            BOOST_CHECK(index >= 0 && index <= 1);
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_random_id_by_weight)
    {
        celeritas::weight weight{};
        weight.add_element(101, 10.0);

        BOOST_CHECK_EQUAL(weight.get_random_id_by_weight(), 101);

        weight.add_element(102, 20.0);
        weight.add_element(103, 30.0);

        const std::set expected_ids{ 101, 102, 103 };

        for (auto i = 0; i < 20; ++i)
        {
            const auto id = weight.get_random_id_by_weight();
            BOOST_CHECK(expected_ids.count(id) == 1);
        }
    }

    BOOST_AUTO_TEST_CASE(test_random_on_empty_weight)
    {
        const celeritas::weight weight{};

        BOOST_CHECK_THROW(std::ignore = weight.get_random_index_by_weight(), celeritas::celeritas_error);
        BOOST_CHECK_THROW(std::ignore = weight.get_random_id_by_weight(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
