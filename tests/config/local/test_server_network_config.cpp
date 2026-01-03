#include "config/local/server_network_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(server_network_config_tests)

    BOOST_AUTO_TEST_CASE(test_server_network_config_default_constructor)
    {
        const celeritas::server_network_config config{};
        BOOST_CHECK(config.get_server_network_type() == celeritas::server_network_type::unknown);
        BOOST_CHECK_EQUAL(config.get_port(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_server_network_config_parameterized_constructor)
    {
        constexpr auto type = celeritas::server_network_type::http;
        constexpr auto port = 8080;

        const celeritas::server_network_config config{ type, port };

        BOOST_CHECK(config.get_server_network_type() == type);
        BOOST_CHECK_EQUAL(config.get_port(), port);
    }

BOOST_AUTO_TEST_SUITE_END()
