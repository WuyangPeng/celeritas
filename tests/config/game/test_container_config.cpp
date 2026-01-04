#include "common/core/celeritas_error.h"
#include "config/game/container_config.tpp"
#include "mock/mock_config_element.h"

#include <boost/test/unit_test.hpp>

#include <memory>
#include <string>

namespace
{
    void test_get(const celeritas::container_config<celeritas::mock_config_element>& container,
                  const int id,
                  const std::string& result)
    {
        const auto optional_result = container.get(id);
        BOOST_CHECK(optional_result.has_value());
        BOOST_CHECK_EQUAL((*optional_result)->get_value(), result);
    }
}

BOOST_AUTO_TEST_SUITE(container_config_suite)

    BOOST_AUTO_TEST_CASE(test_add_and_get)
    {
        celeritas::container_config<celeritas::mock_config_element> container{};

        const auto element1 = std::make_shared<celeritas::mock_config_element>(1, "one");
        const auto element2 = std::make_shared<celeritas::mock_config_element>(2, "two");

        container.add_config(element1);
        container.add_config(element2);

        test_get(container, 1, "one");
        test_get(container, 2, "two");

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

        const auto element = std::make_shared<celeritas::mock_config_element>(10, "ten");
        container.add_config(element);

        const auto& container_config = container.get_container();
        BOOST_CHECK_EQUAL(container_config.size(), 1);
        BOOST_CHECK_EQUAL(container_config.at(10)->get_value(), "ten");
    }

BOOST_AUTO_TEST_SUITE_END()
