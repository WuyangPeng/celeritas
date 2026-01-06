#include "common/core/celeritas_error.h"
#include "common/core/session_route.h"
#include "config/aggregate/app_config.h"
#include "config/basic/server_network_type.h"
#include "fixture/protobuf_handle_parameter_fixture.h"
#include "message/basic/header.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "mock/mock_parameters_application_loader.h"
#include "mock/mock_parameters_resource_loader.h"
#include "mock/mock_parameters_session.h"
#include "service_registry/data/service_info.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>
#include <google/protobuf/empty.pb.h>

BOOST_AUTO_TEST_SUITE(protobuf_handle_parameter_suite)

    BOOST_AUTO_TEST_CASE(test_protobuf_handle_parameter_creation_and_getters)
    {
        const celeritas::header header{ 123, 456 };
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_rpc(), 123);
        BOOST_CHECK_EQUAL(parameter->get_user_id(), 456);
        BOOST_CHECK(parameter->get_protobuf_message() == nullptr);
    }

    BOOST_AUTO_TEST_CASE(test_get_protobuf_message_with_body)
    {
        const celeritas::header header{ 123, 456 };
        const auto message = std::make_shared<google::protobuf::Empty>();
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, message, nullptr, nullptr, nullptr);

        BOOST_CHECK(parameter->get_protobuf_message() != nullptr);
        BOOST_CHECK_EQUAL(parameter->get_protobuf_message(), message);
    }

    BOOST_FIXTURE_TEST_CASE(test_session_related_getters, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();

        BOOST_CHECK_EQUAL(parameter->get_session_id(), 125);
        BOOST_CHECK(parameter->get_server_network_type() == celeritas::server_network_type::tcp);
    }

    BOOST_FIXTURE_TEST_CASE(test_instance_id_getter_and_setter, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();

        parameter->set_instance_id("test_instance");
        BOOST_CHECK_EQUAL(parameter->get_instance_id(), "test_instance");
    }

    BOOST_FIXTURE_TEST_CASE(test_loaders_getters, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();

        BOOST_CHECK_NO_THROW([&parameter] { std::ignore = parameter->get_application_loader(); }());
        BOOST_CHECK_EQUAL(parameter->get_application_loader(), get_application_loader());
        BOOST_CHECK_EQUAL(parameter->get_resource_loader(), get_resource_loader());
    }

    BOOST_FIXTURE_TEST_CASE(test_app_config_getter, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();

        BOOST_CHECK_NO_THROW([&parameter] { std::ignore = parameter->get_app_config(); }());
        const auto app_config = parameter->get_app_config();
        BOOST_CHECK_EQUAL(app_config->get_external_host(), "192.168.1.100");
    }

    BOOST_FIXTURE_TEST_CASE(test_add_session_route, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();

        BOOST_CHECK_NO_THROW([&parameter] { parameter->add_session_route(1, celeritas::session_route{ celeritas::server_network_type::tcp, 1, "instance" }); }());
    }

    BOOST_FIXTURE_TEST_CASE(test_get_any_io_executor, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();
        BOOST_CHECK_NO_THROW([&parameter] { std::ignore = parameter->get_any_io_executor(); }());
        BOOST_CHECK(parameter->get_any_io_executor().target<boost::asio::io_context::executor_type>());
    }

    BOOST_FIXTURE_TEST_CASE(test_write_to_response, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();
        const google::protobuf::Empty empty{};
        parameter->write_to_response(empty);
        BOOST_CHECK_EQUAL(get_session()->get_write_with_header_count(), 1);
        BOOST_CHECK_EQUAL(get_session()->get_last_header().get_rpc(), 123);

        const celeritas::header custom_header{ 789, 101 };
        parameter->write_to_response(custom_header, empty);
        BOOST_CHECK_EQUAL(get_session()->get_write_with_header_count(), 2);
        BOOST_CHECK_EQUAL(get_session()->get_last_header().get_rpc(), 789);
    }

    BOOST_FIXTURE_TEST_CASE(test_write_to_client, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();
        const google::protobuf::Empty empty{};
        parameter->write_to_client(empty);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_write_to_client_count(), 1);
    }

    BOOST_FIXTURE_TEST_CASE(test_write_to_user, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();
        parameter->write_to_user("game_server");
        BOOST_CHECK_EQUAL(get_resource_loader()->get_write_to_user_count(), 1);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_last_write_to_user_server_type(), "game_server");
    }

    BOOST_FIXTURE_TEST_CASE(test_write_to_server, celeritas::protobuf_handle_parameter_fixture)
    {
        const auto parameter = get_parameter();
        const google::protobuf::Empty empty{};

        parameter->write_to_server("logic_server", empty);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_write_to_server_count(), 1);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_last_write_to_server_type(), "logic_server");

        parameter->write_to_server("asset_server", "asset-1", empty);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_write_to_server_count(), 2);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_last_write_to_server_type(), "asset_server");
        BOOST_CHECK_EQUAL(get_resource_loader()->get_last_write_to_server_instance_id(), "asset-1");
    }

    BOOST_FIXTURE_TEST_CASE(test_check_client_method, celeritas::protobuf_handle_parameter_fixture)
    {
        const std::map<std::string, celeritas::service_info> empty_container{};
        get_parameter()->check_client("test_server", empty_container);

        BOOST_CHECK_EQUAL(get_resource_loader()->get_check_client_count(), 1);
        BOOST_CHECK_EQUAL(get_resource_loader()->get_last_check_client_server_type(), "test_server");
    }

    BOOST_AUTO_TEST_CASE(test_null_session_dependency_throws)
    {
        constexpr celeritas::header header{};
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_session_id(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_server_network_type(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_instance_id(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_any_io_executor(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { parameter->set_instance_id("instance"); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { parameter->add_session_route(1, celeritas::session_route{ celeritas::server_network_type::tcp, 1, "instance" }); }(), celeritas::celeritas_error);

        const google::protobuf::Empty empty{};
        BOOST_CHECK_THROW(([&parameter, &empty] { parameter->write_to_response(empty); }()), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_null_application_loader_dependency_throws)
    {
        constexpr celeritas::header header{};
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_application_loader(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_app_config(); }(), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_null_resource_loader_dependency_throws)
    {
        constexpr celeritas::header header{};
        const auto parameter = std::make_shared<celeritas::protobuf_handle_parameter>(header, nullptr, nullptr, nullptr, nullptr);

        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_resource_loader(); }(), celeritas::celeritas_error);

        const google::protobuf::Empty empty{};
        BOOST_CHECK_THROW(([&parameter, &empty] { parameter->write_to_client(empty); }()), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { parameter->write_to_user("user"); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW(([&parameter, &empty] { parameter->write_to_server("server", empty); }()), celeritas::celeritas_error);
        BOOST_CHECK_THROW(([&parameter, &empty] { parameter->write_to_server("server", "instance-1", empty); }()), celeritas::celeritas_error);

        const std::map<std::string, celeritas::service_info> empty_container{};
        BOOST_CHECK_THROW(parameter->check_client("test", empty_container), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
