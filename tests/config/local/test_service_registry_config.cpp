#include "config/local/service_registry_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(service_registry_config_suite)

    BOOST_AUTO_TEST_CASE(test_service_registry_config_default_constructor)
    {
        const celeritas::service_registry_config config{};

        BOOST_CHECK(config.get_name().empty());
        BOOST_CHECK_EQUAL(config.get_host(), celeritas::default_service_registry_host);
        BOOST_CHECK_EQUAL(config.get_port(), celeritas::default_service_registry_port);
    }

    BOOST_AUTO_TEST_CASE(test_service_registry_config_parameterized_constructor)
    {
        const std::string name{ "test_service" };
        const std::string host{ "192.168.1.100" };
        constexpr auto port = 9000;

        const celeritas::service_registry_config config{ name, host, port };

        BOOST_CHECK_EQUAL(config.get_name(), name);
        BOOST_CHECK_EQUAL(config.get_host(), host);
        BOOST_CHECK_EQUAL(config.get_port(), port);
    }

BOOST_AUTO_TEST_SUITE_END()
