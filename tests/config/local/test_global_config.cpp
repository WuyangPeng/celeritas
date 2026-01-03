#include "config/local/global_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(global_config_suite)

    BOOST_AUTO_TEST_CASE(test_global_config_default_constructor)
    {
        const celeritas::global_config config{};
        BOOST_CHECK(config.get_external_host().empty());
    }

    BOOST_AUTO_TEST_CASE(test_global_config_parameterized_constructor)
    {
        const std::string expected_host{ "127.0.0.1" };
        const celeritas::global_config config{ expected_host };
        BOOST_CHECK_EQUAL(config.get_external_host(), expected_host);
    }

BOOST_AUTO_TEST_SUITE_END()
