#include "config/aggregate/server_config.h"
#include "config/local/server_network_config.h"
#include "fixture/server_config_fixture.h"

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE(server_config_suite)

    BOOST_AUTO_TEST_CASE(test_server_config_default_constructor)
    {
        const celeritas::server_config config{};

        BOOST_CHECK(config.get_instance_id().empty());
        BOOST_CHECK(config.get_service_name().empty());
        BOOST_CHECK(config.get_game_server_id().empty());
        BOOST_CHECK(config.get_host().empty());
        BOOST_CHECK_EQUAL(config.get_worker_pool_size(), celeritas::default_worker_pool_size);
        BOOST_CHECK_EQUAL(config.get_datacenter_id(), 0);
        BOOST_CHECK_EQUAL(config.get_worker_id(), 0);
        BOOST_CHECK(!config.is_load_game_config());
        BOOST_CHECK(config.begin() == config.end());
    }

    BOOST_FIXTURE_TEST_CASE(test_server_config_parameterized_constructor_properties, celeritas::server_config_fixture)
    {
        const auto config = create_config();

        BOOST_CHECK_EQUAL(config.get_instance_id(), instance_id);
        BOOST_CHECK_EQUAL(config.get_service_name(), service_name);
        BOOST_CHECK_EQUAL(config.get_game_server_id(), game_server_id);
        BOOST_CHECK_EQUAL(config.get_host(), host);
        BOOST_CHECK_EQUAL(config.get_worker_pool_size(), worker_pool_size);
        BOOST_CHECK_EQUAL(config.get_datacenter_id(), datacenter_id);
        BOOST_CHECK_EQUAL(config.get_worker_id(), worker_id);
        BOOST_CHECK_EQUAL(config.is_load_game_config(), load_game_config);
    }

    BOOST_FIXTURE_TEST_CASE(test_server_config_parameterized_constructor_networks, celeritas::server_config_fixture)
    {
        celeritas::server_config::server_network_container networks{};
        networks.emplace_back(celeritas::server_network_type::http, 8080);
        networks.emplace_back(celeritas::server_network_type::tcp, 9090);

        const auto config = create_config(networks);

        auto iter = config.begin();
        BOOST_CHECK(iter != config.end());
        BOOST_CHECK(iter->get_server_network_type() == celeritas::server_network_type::http);
        BOOST_CHECK_EQUAL(iter->get_port(), 8080);

        ++iter;
        BOOST_CHECK(iter != config.end());
        BOOST_CHECK(iter->get_server_network_type() == celeritas::server_network_type::tcp);
        BOOST_CHECK_EQUAL(iter->get_port(), 9090);

        ++iter;
        BOOST_CHECK(iter == config.end());
    }

    BOOST_AUTO_TEST_CASE(test_is_not_service_registry_server)
    {
        const celeritas::server_config config{ "server_01", "game_service", {}, "game_server_id_01", "localhost", 4, 1, 1, false };
        BOOST_CHECK(!config.is_service_registry_server());
    }

    BOOST_AUTO_TEST_CASE(test_is_service_registry_server)
    {
        const celeritas::server_config config{ "server_02", "service_registry_server", {}, "game_server_id_02", "localhost", 4, 1, 1, false };
        BOOST_CHECK(config.is_service_registry_server());
    }

BOOST_AUTO_TEST_SUITE_END()
