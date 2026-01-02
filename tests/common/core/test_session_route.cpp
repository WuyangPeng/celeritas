#include "common/core/enum_cast.h"
#include "common/core/session_route.h"
#include "config/basic/server_network_type.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(session_route_suite)

    BOOST_AUTO_TEST_CASE(test_session_route_creation)
    {
        constexpr auto expected_net_type = celeritas::server_network_type::tcp;
        constexpr auto expected_session_id = 12345LL;
        const std::string expected_instance_id{ "player-1" };

        const celeritas::session_route route{ expected_net_type, expected_session_id, expected_instance_id };

        BOOST_CHECK_EQUAL(celeritas::enum_cast_underlying(route.get_server_network_type()), celeritas::enum_cast_underlying(expected_net_type));
        BOOST_CHECK_EQUAL(route.get_session_id(), expected_session_id);
        BOOST_CHECK_EQUAL(route.get_instance_id(), expected_instance_id);
    }

BOOST_AUTO_TEST_SUITE_END()
