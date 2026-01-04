#include "common/core/celeritas_error.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "config/local/database_config.h"
#include "config/local/health_check_url_config.h"
#include "config/local/logger_config.h"
#include "config/local/logger_level_config.h"
#include "config/local/service_registry_config.h"
#include "fixture/config_file_fixture.h"

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <fstream>

namespace
{
    void test_database_config(const celeritas::app_config::const_database_shared_ptr& database, const std::string& db_name)
    {
        BOOST_CHECK_EQUAL(database->get_name(), db_name);
        BOOST_CHECK(database->get_database_type() == celeritas::database_type::mysql);
        BOOST_CHECK_EQUAL(database->get_host(), "localhost");
        BOOST_CHECK_EQUAL(database->get_port(), 3306);
        BOOST_CHECK_EQUAL(database->get_user(), "root");
        BOOST_CHECK_EQUAL(database->get_password(), "pass");
        BOOST_CHECK_EQUAL(database->get_db_name(), "test");
        BOOST_CHECK_EQUAL(database->get_min_connections(), 1);
        BOOST_CHECK_EQUAL(database->get_max_connections(), 5);
        BOOST_CHECK_EQUAL(database->get_timeout_seconds(), 3);
        BOOST_CHECK_EQUAL(database->get_expire_seconds(), 3600);
    }

    void test_level_config(const celeritas::app_config& config)
    {
        const auto level = config.get_logger_level_config();
        BOOST_CHECK(level->get_default_level() == celeritas::severity_level_type::info);
        BOOST_CHECK(level->get_console_level() == celeritas::severity_level_type::debug);
    }

    void test_logger_config(const celeritas::app_config::const_logger_shared_ptr& logger)
    {
        BOOST_CHECK(logger->get_severity_level_type() == celeritas::severity_level_type::warning);
        BOOST_CHECK(logger->is_console_enabled());
        BOOST_CHECK_EQUAL(logger->get_channel_name(), "test_chan");
        BOOST_CHECK_EQUAL(logger->get_log_file_name(), "test_log");
        BOOST_CHECK_EQUAL(logger->get_rotation_size(), 100);
    }

    void test_logger_config(const celeritas::app_config& config)
    {
        const std::string logger_name{ "test_logger" };

        const auto loggers = config.get_logger_config();
        BOOST_CHECK(loggers->contains(logger_name));

        if (const auto iter = loggers->find(logger_name);
            iter != loggers->cend())
        {
            test_logger_config(iter->second);
        }
    }

    void test_service_registry_config(const celeritas::app_config::const_service_registry_shared_ptr& registry, const std::string& registry_name)
    {
        BOOST_CHECK_EQUAL(registry->get_name(), registry_name);
        BOOST_CHECK_EQUAL(registry->get_host(), "127.0.0.1");
        BOOST_CHECK_EQUAL(registry->get_port(), 5000);
    }

    void test_service_registry_config(const celeritas::app_config& config)
    {
        const std::string registry_name{ "registry_1" };

        const auto registry = config.get_service_registry_config();
        BOOST_CHECK(registry->contains(registry_name));

        if (const auto iter = registry->find(registry_name);
            iter != registry->cend())
        {
            test_service_registry_config(iter->second, registry_name);
        }
    }

    void test_server_config(const celeritas::app_config& config)
    {
        const auto server = config.get_server_config();
        BOOST_CHECK_EQUAL(server->get_instance_id(), "server_1");
        BOOST_CHECK_EQUAL(server->get_service_name(), "game");
        BOOST_CHECK_EQUAL(server->get_game_server_id(), "game_server_id_1");
        BOOST_CHECK_EQUAL(server->get_host(), "0.0.0.0");
        BOOST_CHECK_EQUAL(server->get_worker_pool_size(), 4);
        BOOST_CHECK_EQUAL(server->get_datacenter_id(), 1);
        BOOST_CHECK_EQUAL(server->get_worker_id(), 2);
        BOOST_CHECK(server->is_load_game_config());

        const auto iter = server->begin();
        BOOST_CHECK(iter != server->end());
        BOOST_CHECK(iter->get_server_network_type() == celeritas::server_network_type::http);
        BOOST_CHECK_EQUAL(iter->get_port(), 8080);
    }
}

BOOST_AUTO_TEST_SUITE(app_config_suite)

    BOOST_AUTO_TEST_CASE(test_load_global_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_global();

        celeritas::app_config config{};
        config.load_global_config(fixture.get_filename());

        BOOST_CHECK_EQUAL(config.get_external_host(), "192.168.1.100");
    }

    BOOST_AUTO_TEST_CASE(test_load_health_check_url_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_health_check_url();

        celeritas::app_config config{};
        config.load_health_check_url_config(fixture.get_filename());

        const auto health_config = config.get_health_check_url_config();
        BOOST_CHECK_EQUAL(health_config->get_url(), "/status");
        BOOST_CHECK_EQUAL(health_config->get_interval(), 60);
        BOOST_CHECK_EQUAL(health_config->get_timeout(), 10);
    }

    BOOST_AUTO_TEST_CASE(test_load_databases_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_databases();

        celeritas::app_config config{};
        config.load_databases_config(fixture.get_filename());

        const std::string db_name{ "test_db" };

        const auto database_config = config.get_database_config(db_name);
        test_database_config(database_config, db_name);

        const auto current_time = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK(config.get_expire_milliseconds(db_name) >= current_time + database_config->get_expire_seconds() * celeritas::milliseconds);

        BOOST_CHECK_THROW([&config] { std::ignore = config.get_database_config("non_existent");}(), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_load_loggers_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_loggers();

        celeritas::app_config config{};
        config.load_loggers_config(fixture.get_filename());

        test_level_config(config);
        test_logger_config(config);
    }

    BOOST_AUTO_TEST_CASE(test_load_service_registry_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_service_registry();

        celeritas::app_config config{};
        config.load_service_registry_config(fixture.get_filename());

        test_service_registry_config(config);
    }

    BOOST_AUTO_TEST_CASE(test_load_server_config)
    {
        const auto fixture = celeritas::config_file_fixture::get_server();

        celeritas::app_config config{};
        config.load_server_config(fixture.get_filename());

        test_server_config(config);
    }

BOOST_AUTO_TEST_SUITE_END()
