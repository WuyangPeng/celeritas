#include "common/core/celeritas_error.h"
#include "config/game/container_config.tpp"
#include "mock/mock_config_element.h"

#include <boost/test/unit_test.hpp>

#include <memory>
#include <string>

BOOST_AUTO_TEST_SUITE(container_config_suite)

    BOOST_AUTO_TEST_CASE(test_add_and_get)
    {
        celeritas::container_config<celeritas::mock_config_element> container{};

        const auto element1 = std::make_shared<celeritas::mock_config_element>(1, "one");
        const auto element2 = std::make_shared<celeritas::mock_config_element>(2, "two");

        container.add_config(element1);
        container.add_config(element2);

        const auto result1 = container.get(1);
        BOOST_CHECK(result1.has_value());
        BOOST_CHECK_EQUAL((*result1)->get_value(), "one");

        const auto result2 = container.get(2);
        BOOST_CHECK(result2.has_value());
        BOOST_CHECK_EQUAL((*result2)->get_value(), "two");

        const auto result3 = container.get(3);
        BOOST_CHECK(!result3.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_duplicate_id_throws)
    {
        celeritas::container_config<celeritas::mock_config_element> container{};

        const auto element1 = std::make_shared<celeritas::mock_config_element>(1, "first");
        const auto element2 = std::make_shared<celeritas::mock_config_element>(1, "duplicate");

        container.add_config(element1);

        BOOST_CHECK_THROW(container.add_config(element2), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_get_container)
    {
        celeritas::container_config<celeritas::mock_config_element> container{};

        const auto element1 = std::make_shared<celeritas::mock_config_element>(10, "ten");
        container.add_config(element1);

        const auto& container_config = container.get_container();
        BOOST_CHECK_EQUAL(container_config.size(), 1);
        BOOST_CHECK_EQUAL(container_config.at(10)->get_value(), "ten");
    }

BOOST_AUTO_TEST_SUITE_END()
