#include "config/server_network_type.h"
#include "service_registry/data/service_info.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(service_info_suite)

    BOOST_AUTO_TEST_CASE(test_constructor_and_getters)
    {
        const std::string instance_id{ "instance_1" };
        const std::string service_name{ "test_service" };
        const std::string host{ "127.0.0.1" };
        const std::string external_host{ "example.com" };
        const std::string game_server_id{ "game_server_1" };
        const celeritas::service_info::protocol_port_container ports{ { celeritas::server_network_type::tcp, 8080 },
                                                                      { celeritas::server_network_type::http, 8081 } };
        constexpr auto start_time = 1234567890;

        const celeritas::service_info info(instance_id, service_name, host, external_host, game_server_id, ports, start_time);

        BOOST_CHECK_EQUAL(info.get_instance_id(), instance_id);
        BOOST_CHECK_EQUAL(info.get_service_name(), service_name);
        BOOST_CHECK_EQUAL(info.get_host(), host);
        BOOST_CHECK_EQUAL(info.get_external_host(), external_host);
        BOOST_CHECK_EQUAL(info.get_game_server_id(), game_server_id);
        BOOST_CHECK_EQUAL(info.get_start_server_time(), start_time);
        BOOST_CHECK_EQUAL(info.get_port(celeritas::server_network_type::tcp), 8080);
        BOOST_CHECK_EQUAL(info.get_port(celeritas::server_network_type::http), 8081);
        BOOST_CHECK_EQUAL(info.get_port(celeritas::server_network_type::websocket), 0);
    }

    BOOST_AUTO_TEST_CASE(test_setters)
    {
        celeritas::service_info info{};

        info.set_health_check_level_type(celeritas::health_check_level_type::health);
        BOOST_CHECK(info.get_health_check_level_type() == celeritas::health_check_level_type::health);

        const auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        info.set_last_heartbeat(now);

        const auto last_heartbeat = std::chrono::duration_cast<std::chrono::seconds>(info.get_last_heartbeat().time_since_epoch()).count();
        BOOST_CHECK_GE(last_heartbeat, 0);
    }

BOOST_AUTO_TEST_SUITE_END()
