#include "config/local/logger_config.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(logger_config_suite)

    BOOST_AUTO_TEST_CASE(test_logger_config_default_constructor)
    {
        const celeritas::logger_config config{};

        BOOST_CHECK(config.get_name().empty());
        BOOST_CHECK(config.get_channel_name().empty());
        BOOST_CHECK(config.get_log_file_name().empty());
        BOOST_CHECK(config.get_severity_level_type() == celeritas::severity_level_type::trace);
        BOOST_CHECK_EQUAL(config.get_rotation_size(), celeritas::default_logger_rotation_size);
        BOOST_CHECK(!config.is_console_enabled());
    }

    BOOST_AUTO_TEST_CASE(test_logger_config_parameterized_constructor)
    {
        const std::string name{ "test_logger" };
        constexpr auto severity = celeritas::severity_level_type::info;
        constexpr auto console_enabled = true;
        const std::string channel_name{ "test_channel" };
        const std::string log_file_name{ "test.log" };
        constexpr auto rotation_size = 1024;

        const celeritas::logger_config config{ name, severity, console_enabled, channel_name, log_file_name, rotation_size };

        BOOST_CHECK_EQUAL(config.get_name(), name);
        BOOST_CHECK(config.get_severity_level_type() == severity);
        BOOST_CHECK_EQUAL(config.is_console_enabled(), console_enabled);
        BOOST_CHECK_EQUAL(config.get_channel_name(), channel_name);
        BOOST_CHECK_EQUAL(config.get_log_file_name(), log_file_name);
        BOOST_CHECK_EQUAL(config.get_rotation_size(), rotation_size);
    }

    BOOST_AUTO_TEST_CASE(test_get_severity_level_type_from_string)
    {
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("trace") == celeritas::severity_level_type::trace);
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("debug") == celeritas::severity_level_type::debug);
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("info") == celeritas::severity_level_type::info);
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("warning") == celeritas::severity_level_type::warning);
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("error") == celeritas::severity_level_type::error);
        BOOST_CHECK(celeritas::logger_config::get_severity_level_type("fatal") == celeritas::severity_level_type::fatal);
    }

    BOOST_AUTO_TEST_CASE(test_get_severity_level_type_invalid)
    {
        BOOST_CHECK_THROW([] { std::ignore = celeritas::logger_config::get_severity_level_type("unknown"); }(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
