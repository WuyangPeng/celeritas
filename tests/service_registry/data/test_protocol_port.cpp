#include "config/server_network_type.h"
#include "service_registry/data/protocol_port.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(protocol_port_suite)

    BOOST_AUTO_TEST_CASE(test_constructor_and_getters)
    {
        constexpr auto type = celeritas::server_network_type::tcp;
        constexpr auto port = 8080;

        const celeritas::protocol_port protocol_port{ type, port };

        BOOST_CHECK(protocol_port.get_server_network_type() == type);
        BOOST_CHECK_EQUAL(protocol_port.get_port(), port);
    }

BOOST_AUTO_TEST_SUITE_END()
