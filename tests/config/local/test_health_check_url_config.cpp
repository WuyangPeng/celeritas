#include "config/local/health_check_url_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(health_check_url_config_suite)

    BOOST_AUTO_TEST_CASE(test_health_check_url_config_default_constructor)
    {
        const celeritas::health_check_url_config config{};
        BOOST_CHECK(config.get_url().empty());
        BOOST_CHECK_EQUAL(config.get_interval(), celeritas::default_health_check_interval);
    }

    BOOST_AUTO_TEST_CASE(test_health_check_url_config_parameterized_constructor)
    {
        const std::string url{ "/health" };
        constexpr auto interval = 5000;
        constexpr auto timeout = 1000;

        const celeritas::health_check_url_config config{ url, interval, timeout };

        BOOST_CHECK_EQUAL(config.get_url(), url);
        BOOST_CHECK_EQUAL(config.get_interval(), interval);
    }

BOOST_AUTO_TEST_SUITE_END()
