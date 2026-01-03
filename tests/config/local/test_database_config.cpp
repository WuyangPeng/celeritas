#include "config/local/database_config.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_config_suite)

    BOOST_AUTO_TEST_CASE(test_database_config_default_constructor)
    {
        const celeritas::database_config config{};

        BOOST_CHECK(config.get_name().empty());
        BOOST_CHECK(config.get_database_type() == celeritas::database_type::unknown);
        BOOST_CHECK(config.get_host().empty());
        BOOST_CHECK_EQUAL(config.get_port(), 0);
        BOOST_CHECK(config.get_user().empty());
        BOOST_CHECK(config.get_password().empty());
        BOOST_CHECK(config.get_db_name().empty());
        BOOST_CHECK_EQUAL(config.get_min_connections(), celeritas::default_database_min_connections);
        BOOST_CHECK_EQUAL(config.get_max_connections(), celeritas::default_database_max_connections);
        BOOST_CHECK_EQUAL(config.get_timeout_seconds(), celeritas::default_database_timeout_seconds);
        BOOST_CHECK_EQUAL(config.get_expire_seconds(), celeritas::default_database_expire_seconds);
    }

    BOOST_AUTO_TEST_CASE(test_database_config_parameterized_constructor)
    {
        const std::string name{ "test_db" };
        constexpr auto type = celeritas::database_type::mysql;
        const std::string host{ "localhost" };
        constexpr auto port = 3306;
        const std::string user{ "root" };
        const std::string password{ "password" };
        const std::string db_name{ "test_schema" };
        constexpr auto min_conn = 5;
        constexpr auto max_conn = 20;
        constexpr auto timeout = 30;
        constexpr auto expire = 600;

        const celeritas::database_config config{ name, type, host, port, user, password, db_name, min_conn, max_conn, timeout, expire };

        BOOST_CHECK_EQUAL(config.get_name(), name);
        BOOST_CHECK(config.get_database_type() == type);
        BOOST_CHECK_EQUAL(config.get_host(), host);
        BOOST_CHECK_EQUAL(config.get_port(), port);
        BOOST_CHECK_EQUAL(config.get_user(), user);
        BOOST_CHECK_EQUAL(config.get_password(), password);
        BOOST_CHECK_EQUAL(config.get_db_name(), db_name);
        BOOST_CHECK_EQUAL(config.get_min_connections(), min_conn);
        BOOST_CHECK_EQUAL(config.get_max_connections(), max_conn);
        BOOST_CHECK_EQUAL(config.get_timeout_seconds(), timeout);
        BOOST_CHECK_EQUAL(config.get_expire_seconds(), expire);
    }

BOOST_AUTO_TEST_SUITE_END()
