#include "common/framework/mock/mock_framework_application_loader.h"
#include "common/framework/mock/mock_framework_resource_loader.h"
#include "common/framework/mock/mock_framework_session.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "common/core/celeritas_error.h"
#include "common/core/session_route.h"
#include "config/basic/server_network_type.h"
#include "message/basic/header.h"

#include <boost/test/unit_test.hpp>
#include <google/protobuf/any.pb.h>
#include <tuple>

BOOST_AUTO_TEST_SUITE(protobuf_handle_parameter_suite)

    BOOST_AUTO_TEST_CASE(test_protobuf_handle_parameter_creation_and_getters)
    {
        const celeritas::header header{ 123, 456 };
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_rpc(), 123);
        BOOST_CHECK_EQUAL(parameter->get_user_id(), 456);
        BOOST_CHECK(parameter->get_protobuf_message() == nullptr);
    }

    BOOST_AUTO_TEST_CASE(test_protobuf_handle_parameter_with_mocks)
    {
        const celeritas::header header{ 123, 456 };
        const auto session = std::make_shared<celeritas::mock_framework_session>();
        const auto resource_loader = std::make_shared<celeritas::mock_framework_resource_loader>();
        const auto application_loader = std::make_shared<celeritas::mock_framework_application_loader>();
        const auto message = std::make_shared<google::protobuf::Any>();

        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, message, session, resource_loader, application_loader);

        // Test session delegation
        BOOST_CHECK_EQUAL(parameter->get_session_id(), 123);
        BOOST_CHECK(parameter->get_server_network_type() == celeritas::server_network_type::tcp);
        
        parameter->set_instance_id("test_instance");
        BOOST_CHECK_EQUAL(parameter->get_instance_id(), "test_instance");

        // Test resource loader delegation
        // write_to_server returns true in mock
        parameter->write_to_server("game_server", *message);
        parameter->write_to_user("game_server");

        // Test application loader delegation
        BOOST_CHECK_NO_THROW(parameter->get_application_loader());
    }

    BOOST_AUTO_TEST_CASE(test_protobuf_handle_parameter_null_dependency_throws)
    {
        constexpr celeritas::header header{};
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        // Session dependency
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_session_id(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_server_network_type(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_instance_id(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_any_io_executor(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { parameter->set_instance_id("instance"); }(), celeritas::celeritas_error);

        // Resource loader dependency
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_app_config(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { parameter->add_session_route(1, celeritas::session_route{ celeritas::server_network_type::tcp, 1, "instance" }); }(), celeritas::celeritas_error);

        // Application loader dependency
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_application_loader(); }(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
