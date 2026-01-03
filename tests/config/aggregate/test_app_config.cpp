#include "common/core/celeritas_error.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "fixture/config_file_fixture.h"

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <fstream>

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
        BOOST_CHECK_EQUAL(health_config.get_url(), "/status");
        BOOST_CHECK_EQUAL(health_config.get_interval(), 60);
        BOOST_CHECK_EQUAL(health_config.get_timeout(), 10);
    }

    BOOST_AUTO_TEST_CASE(test_load_databases_config)
    {
        std::string content = R"(<?xml version="1.0" encoding="utf-8"?>
<databases>
    <database name="test_db">
        <database_type>mysql</database_type>
        <host>localhost</host>
        <port>3306</port>
        <user>root</user>
        <password>pass</password>
        <db_name>test</db_name>
        <min_connections>1</min_connections>
        <max_connections>5</max_connections>
        <timeout_seconds>3</timeout_seconds>
        <expire_seconds>3600</expire_seconds>
    </database>
</databases>)";
        std::string filename = "test_databases.xml";
        celeritas::config_file_fixture fixture{ filename, content };

        celeritas::app_config config;
        config.load_databases_config(filename);

        auto database_config = config.get_database_config("test_db");
        BOOST_CHECK_EQUAL(database_config.get_name(), "test_db");
        BOOST_CHECK(database_config.get_database_type() == celeritas::database_type::mysql);
        BOOST_CHECK_EQUAL(database_config.get_host(), "localhost");
        BOOST_CHECK_EQUAL(database_config.get_expire_seconds(), 3600);

        const auto current_time = celeritas::time_helper::get_current_milliseconds();

        // 测试 get_expire_milliseconds
        // 注意：此测试依赖于当前时间，因此我们检查它是否大致正确（大于当前时间）
        BOOST_CHECK(config.get_expire_milliseconds("test_db") >= current_time + database_config.get_expire_seconds() * celeritas::milliseconds);

        // 测试不存在的数据库
        BOOST_CHECK_THROW([config] { std::ignore = config.get_database_config("non_existent");}(), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_load_loggers_config)
    {
        std::string content = R"(<?xml version="1.0" encoding="utf-8"?>
<loggers>
    <global_level>info</global_level>
    <console_level>debug</console_level>
    <logger name="test_logger">
        <level>warning</level>
        <console_enabled>true</console_enabled>
        <channel_name>test_chan</channel_name>
        <log_file_name>test_log</log_file_name>
        <rotation_size>100</rotation_size>
    </logger>
</loggers>)";
        std::string filename = "test_loggers.xml";
        celeritas::config_file_fixture fixture{ filename, content };

        celeritas::app_config config;
        config.load_loggers_config(filename);

        auto level_config = config.get_logger_level_config();
        BOOST_CHECK(level_config.get_default_level() == celeritas::severity_level_type::info);
        BOOST_CHECK(level_config.get_console_level() == celeritas::severity_level_type::debug);

        auto loggers = config.get_logger_config();
        BOOST_CHECK(loggers.find("test_logger") != loggers.end());

        auto logger = loggers["test_logger"];
        BOOST_CHECK(logger.get_severity_level_type() == celeritas::severity_level_type::warning);
        BOOST_CHECK_EQUAL(logger.get_channel_name(), "test_chan");
    }

    BOOST_AUTO_TEST_CASE(test_load_service_registry_config)
    {
        std::string content = R"(<?xml version="1.0" encoding="utf-8"?>
<service_registry>
    <network instance_id="reg_1">
        <host>127.0.0.1</host>
        <port>5000</port>
    </network>
</service_registry>)";
        std::string filename = "test_service_registry.xml";
        celeritas::config_file_fixture fixture{ filename, content };

        celeritas::app_config config;
        config.load_service_registry_config(filename);

        auto registry_configs = config.get_service_registry_config();
        BOOST_CHECK(registry_configs.find("reg_1") != registry_configs.end());

        auto reg_config = registry_configs["reg_1"];
        BOOST_CHECK_EQUAL(reg_config.get_host(), "127.0.0.1");
        BOOST_CHECK_EQUAL(reg_config.get_port(), 5000);
    }

    BOOST_AUTO_TEST_CASE(test_load_server_config)
    {
        // 假设 server_config_reader 期望特定的 XML 结构。
        // 基于 server_config 类，它可能需要 instance_id, service_name 等。
        // 我们基于服务器配置的典型结构构建一个最小的有效 XML。
        std::string content = R"(<?xml version="1.0" encoding="utf-8"?>
<server>
    <instance_id>srv_1</instance_id>
    <service_name>game</service_name>
    <game_server_id>gs_1</game_server_id>
    <host>0.0.0.0</host>
    <worker_pool_size>4</worker_pool_size>
    <datacenter_id>1</datacenter_id>
    <worker_id>1</worker_id>
    <load_game_config>true</load_game_config>
    <network>
        <network_type>http</type>
        <port>8080</port>
    </network>
</server>)";
        std::string filename = "test_server.xml";
        celeritas::config_file_fixture fixture{ filename, content };

        celeritas::app_config config;
        config.load_server_config(filename);

        auto server_cfg = config.get_server_config();
        BOOST_CHECK_EQUAL(server_cfg.get_instance_id(), "srv_1");
        BOOST_CHECK_EQUAL(server_cfg.get_service_name(), "game");
        BOOST_CHECK_EQUAL(server_cfg.get_worker_pool_size(), 4);
        BOOST_CHECK(server_cfg.is_load_game_config());

        auto it = server_cfg.begin();
        BOOST_CHECK(it != server_cfg.end());
        BOOST_CHECK(it->get_server_network_type() == celeritas::server_network_type::http);
        BOOST_CHECK_EQUAL(it->get_port(), 8080);
    }

BOOST_AUTO_TEST_SUITE_END()
