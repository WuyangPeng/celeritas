#include "config_file_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <filesystem>
#include <fstream>

celeritas::config_file_fixture::config_file_fixture(std::string name, const std::string& content)
    : filename_{ std::move(name) }
{
    init(content);
}

celeritas::config_file_fixture::~config_file_fixture() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   remove_file();
                               },
                               config_channel,
                               "remove file error: ");
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_global()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<global>
    <external_host>192.168.1.100</external_host>
</global>)" };

    const std::string filename{ "test_global.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_health_check_url()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<health_check_url>
    <url>/status</url>
    <interval>60</interval>
    <timeout>10</timeout>
</health_check_url>)" };
    const std::string filename{ "test_health_check.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_databases()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
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
</databases>)" };
    const std::string filename{ "test_databases.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_loggers()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
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
</loggers>)" };
    const std::string filename{ "test_loggers.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_service_registry()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<service_registry>
    <network instance_id="registry_1">
        <host>127.0.0.1</host>
        <port>5000</port>
    </network>
</service_registry>)" };
    const std::string filename{ "test_service_registry.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_server()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<server>
    <instance_id>server_1</instance_id>
    <service_name>game</service_name>
    <game_server_id>game_server_id_1</game_server_id>
    <host>0.0.0.0</host>
    <worker_pool_size>4</worker_pool_size>
    <datacenter_id>1</datacenter_id>
    <worker_id>2</worker_id>
    <load_game_config>true</load_game_config>
    <network>
        <network_type>http</type>
        <port>8080</port>
    </network>
</server>)" };
    const std::string filename{ "test_server.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_server_invalid_datacenter_id()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<server>
    <instance_id>server_1</instance_id>
    <service_name>game</service_name>
    <host>0.0.0.0</host>
    <datacenter_id>100</datacenter_id>
    <worker_id>2</worker_id>
</server>)" };
    const std::string filename{ "test_server_invalid_dc.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_server_invalid_worker_id()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<server>
    <instance_id>server_1</instance_id>
    <service_name>game</service_name>
    <host>0.0.0.0</host>
    <datacenter_id>1</datacenter_id>
    <worker_id>300</worker_id>
</server>)" };
    const std::string filename{ "test_server_invalid_worker.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_server_invalid_network_type()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<server>
    <instance_id>server_1</instance_id>
    <service_name>game</service_name>
    <host>0.0.0.0</host>
    <datacenter_id>1</datacenter_id>
    <worker_id>2</worker_id>
    <network>
        <network_type>invalid_type</network_type>
        <port>8080</port>
    </network>
</server>)" };
    const std::string filename{ "test_server_invalid_network.xml" };
    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_databases_redis_invalid_expire()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<databases>
    <database name="test_redis">
        <database_type>redis</database_type>
        <host>localhost</host>
        <port>6379</port>
        <expire_seconds>0</expire_seconds>
    </database>
</databases>)" };
    const std::string filename{ "test_databases_invalid_redis.xml" };

    return config_file_fixture{ filename, content };
}

std::string celeritas::config_file_fixture::get_filename() const
{
    return filename_;
}

void celeritas::config_file_fixture::init(const std::string& content) const
{
    std::ofstream out{ filename_ };
    out << content;
    out.close();
}

void celeritas::config_file_fixture::remove_file() const
{
    if (std::filesystem::exists(filename_))
    {
        std::filesystem::remove(filename_);
    }
}
