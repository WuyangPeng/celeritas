#include "config/local/logger_level_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(logger_level_config_tests)

    BOOST_AUTO_TEST_CASE(test_logger_level_config_default)
    {
        constexpr celeritas::logger_level_config config{};

        BOOST_CHECK(!config.is_set_default_level());
        BOOST_CHECK(!config.is_set_console_level());
    }

    BOOST_AUTO_TEST_CASE(test_logger_level_config_set_get)
    {
        celeritas::logger_level_config config{};

        config.set_default_level(celeritas::severity_level_type::info);
        BOOST_CHECK(config.is_set_default_level());
        BOOST_CHECK(config.get_default_level() == celeritas::severity_level_type::info);

        config.set_console_level(celeritas::severity_level_type::debug);
        BOOST_CHECK(config.is_set_console_level());
        BOOST_CHECK(config.get_console_level() == celeritas::severity_level_type::debug);
    }

BOOST_AUTO_TEST_SUITE_END()
