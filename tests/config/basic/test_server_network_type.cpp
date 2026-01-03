#include "config/basic/server_network_type.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(server_network_type_suite)

    BOOST_AUTO_TEST_CASE(test_get_server_network_type_valid)
    {
        BOOST_CHECK(celeritas::get_server_network_type("tcp") == celeritas::server_network_type::tcp);
        BOOST_CHECK(celeritas::get_server_network_type("http") == celeritas::server_network_type::http);
        BOOST_CHECK(celeritas::get_server_network_type("websocket") == celeritas::server_network_type::websocket);
        BOOST_CHECK(celeritas::get_server_network_type("tcp_ssl") == celeritas::server_network_type::tcp_ssl);
        BOOST_CHECK(celeritas::get_server_network_type("https") == celeritas::server_network_type::https);
        BOOST_CHECK(celeritas::get_server_network_type("websocket_secure") == celeritas::server_network_type::websocket_secure);
    }

    BOOST_AUTO_TEST_CASE(test_get_server_network_type_invalid)
    {
        BOOST_CHECK_THROW([] { std::ignore = celeritas::get_server_network_type("unknown"); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([] { std::ignore = celeritas::get_server_network_type(""); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([] { std::ignore = celeritas::get_server_network_type("ws"); }(), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_get_all_server_network_type)
    {
        const auto& all_types = celeritas::get_all_server_network_type();
        BOOST_CHECK_EQUAL(all_types.size(), 6);
    }

BOOST_AUTO_TEST_SUITE_END()
