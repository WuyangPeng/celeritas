#include "config/server_network_type.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/health_check_level_type.h"
#include "service_registry/data/protocol_port.h"
#include "service_registry/data/service_info.h"

#include <boost/test/unit_test.hpp>
#include <vector>

namespace
{
    celeritas::service_info create_service_info(const std::string& instance_id,
                                                const std::string& service_name,
                                                const std::string& game_server_id,
                                                const celeritas::health_check_level_type health)
    {
        celeritas::service_info info{ instance_id,
                                      service_name,
                                      "127.0.0.1",
                                      "host.external",
                                      game_server_id,
                                      { { celeritas::server_network_type::tcp, 1234 } },
                                      1234567890 };
        info.set_health_check_level_type(health);
        return info;
    }

    struct service_registry_fixture
    {
        ~service_registry_fixture()
        {
            celeritas::service_registry::clear_services("test_service");
            celeritas::service_registry::clear_services("idle_service_test");
            celeritas::service_registry::clear_services("health_test_service");
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(service_registry_suite, service_registry_fixture)

    BOOST_AUTO_TEST_CASE(test_register_and_get_service)
    {
        const std::string service_name{ "test_service" };
        auto info1 = create_service_info("instance1", service_name, "", celeritas::health_check_level_type::health);
        auto info2 = create_service_info("instance2", service_name, "", celeritas::health_check_level_type::health);

        celeritas::service_registry::register_service(info1);
        celeritas::service_registry::register_service(info2);

        auto services = celeritas::service_registry::get_services(service_name);
        BOOST_CHECK_EQUAL(services.size(), 2);

        auto service1 = celeritas::service_registry::get_services_by_instance_id("instance1");
        BOOST_CHECK(service1.has_value());
        BOOST_CHECK_EQUAL(service1->get_instance_id(), "instance1");

        auto non_existent_service = celeritas::service_registry::get_services_by_instance_id("non_existent");
        BOOST_CHECK(!non_existent_service.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_clear_services)
    {
        const std::string service_name{ "test_service" };
        celeritas::service_registry::register_service(create_service_info("clear_instance", service_name, "", celeritas::health_check_level_type::health));

        BOOST_CHECK_EQUAL(celeritas::service_registry::get_services(service_name).size(), 1);
        celeritas::service_registry::clear_services(service_name);
        BOOST_CHECK_EQUAL(celeritas::service_registry::get_services(service_name).size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_remove_instance)
    {
        const std::string service_name{ "test_service" };
        celeritas::service_registry::register_service(create_service_info("instance_to_remove", service_name, "", celeritas::health_check_level_type::health));
        celeritas::service_registry::register_service(create_service_info("instance_to_keep", service_name, "", celeritas::health_check_level_type::health));

        BOOST_CHECK_EQUAL(celeritas::service_registry::get_services(service_name).size(), 2);

        celeritas::service_registry::remove_instance("instance_to_remove");
        const auto services = celeritas::service_registry::get_services(service_name);
        BOOST_CHECK_EQUAL(services.size(), 1);
        BOOST_CHECK_EQUAL(services[0].get_instance_id(), "instance_to_keep");
    }

    BOOST_AUTO_TEST_CASE(test_get_idle_services)
    {
        const std::string service_name{ "idle_service_test" };
        celeritas::service_registry::register_service(create_service_info("idle1", service_name, "game1", celeritas::health_check_level_type::health));
        celeritas::service_registry::register_service(create_service_info("idle2", service_name, "game2", celeritas::health_check_level_type::health));
        celeritas::service_registry::register_service(create_service_info("unhealthy", service_name, "game3", celeritas::health_check_level_type::crash));

        auto idle_services = celeritas::service_registry::get_idle_services(service_name);
        BOOST_CHECK_EQUAL(idle_services.size(), 2);

        auto idle_service1 = celeritas::service_registry::get_idle_services(service_name, "game1");
        BOOST_CHECK(idle_service1.has_value());
        BOOST_CHECK_EQUAL(idle_service1->get_instance_id(), "idle1");

        auto idle_service3 = celeritas::service_registry::get_idle_services(service_name, "game3");
        BOOST_CHECK(!idle_service3.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_set_service_health)
    {
        const std::string service_name{ "health_test_service" };
        celeritas::service_registry::register_service(create_service_info("health_instance", service_name, "", celeritas::health_check_level_type::health));

        auto service_before = celeritas::service_registry::get_services_by_instance_id("health_instance");
        BOOST_CHECK(service_before->get_health_check_level_type() == celeritas::health_check_level_type::health);

        celeritas::service_registry::set_service_health("health_instance", celeritas::health_check_level_type::unhealthy);

        auto service_after = celeritas::service_registry::get_services_by_instance_id("health_instance");
        BOOST_CHECK(service_after->get_health_check_level_type() == celeritas::health_check_level_type::unhealthy);
    }

BOOST_AUTO_TEST_SUITE_END()
