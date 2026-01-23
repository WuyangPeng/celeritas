#include "network/core/message_header.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(message_header_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        constexpr celeritas::message_header header{};

        BOOST_CHECK_EQUAL(header.get_header_type(), 0);
        BOOST_CHECK_EQUAL(header.get_header_size(), 0);
        BOOST_CHECK_EQUAL(header.get_body_size(), 0);
        BOOST_CHECK_EQUAL(header.get_total_size(), 0);
        BOOST_CHECK(!header.is_effective());
    }

    BOOST_AUTO_TEST_CASE(test_static_size)
    {
        BOOST_CHECK_EQUAL(celeritas::message_header::get_self_size(), 8);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        constexpr uint16_t header_size{ 100 };
        constexpr uint32_t body_size{ 500 };
        const celeritas::message_header header{ header_size, body_size };

        BOOST_CHECK_EQUAL(header.get_header_size(), header_size);
        BOOST_CHECK_EQUAL(header.get_body_size(), body_size);
        BOOST_CHECK_EQUAL(header.get_total_size(), header_size + body_size);
        BOOST_CHECK(header.is_effective());
    }

    BOOST_AUTO_TEST_CASE(test_network_host_conversion)
    {
        constexpr uint16_t header_size{ 123 };
        constexpr uint32_t body_size{ 456 };
        celeritas::message_header header{ header_size, body_size };

        header.host_to_network();
        header.network_to_host();

        BOOST_CHECK_EQUAL(header.get_header_size(), header_size);
        BOOST_CHECK_EQUAL(header.get_body_size(), body_size);
    }

BOOST_AUTO_TEST_SUITE_END()
