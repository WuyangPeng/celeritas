#include "service_registry/data/health_check.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(health_check_suite)

    BOOST_AUTO_TEST_CASE(test_constructor_and_getters)
    {
        const std::string instance_id{ "test_instance" };
        constexpr auto level = celeritas::health_check_level_type::crash;

        const celeritas::health_check check{ instance_id, level };

        BOOST_CHECK_EQUAL(check.get_instance_id(), instance_id);
        BOOST_CHECK_EQUAL(static_cast<int>(check.get_health_check_level()), static_cast<int>(level));
    }

    BOOST_AUTO_TEST_CASE(test_setters)
    {
        celeritas::health_check check{};

        const std::string instance_id{ "new_instance" };
        check.set_instance_id(instance_id);
        BOOST_CHECK_EQUAL(check.get_instance_id(), instance_id);

        constexpr auto level = celeritas::health_check_level_type::unhealthy;
        check.set_health_check_level(level);
        BOOST_CHECK_EQUAL(static_cast<int>(check.get_health_check_level()), static_cast<int>(level));
    }

    BOOST_AUTO_TEST_CASE(test_json_serialization)
    {
        const std::string instance_id{ "json_instance" };
        constexpr auto level = celeritas::health_check_level_type::health;
        const celeritas::health_check check{ instance_id, level };

        const auto json_string = check.to_json_string();

        const auto check_from_json = celeritas::health_check::from_json_string(json_string);

        BOOST_CHECK_EQUAL(check_from_json.get_instance_id(), instance_id);
        BOOST_CHECK_EQUAL(static_cast<int>(check_from_json.get_health_check_level()), static_cast<int>(level));
    }

BOOST_AUTO_TEST_SUITE_END()
