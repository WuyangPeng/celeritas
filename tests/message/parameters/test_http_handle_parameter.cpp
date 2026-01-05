#include "common/core/celeritas_error.h"
#include "common/framework/mock/mock_framework_application_loader.h"
#include "common/framework/mock/mock_framework_resource_loader.h"
#include "common/framework/mock/mock_framework_session.h"
#include "message/parameters/http_handle_parameter.h"

#include <boost/url.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(http_handle_parameter_suite)

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_creation_and_getters)
    {
        const std::string path{ "/test/path" };
        const std::string params{ "key=value" };
        const auto parameter = std::make_shared<celeritas::http_handle_parameter>(path, params, nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_path(), path);
        BOOST_CHECK_EQUAL(parameter->get_response(), params);
    }

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_get_param)
    {
        const std::string path{ "/test/path" };
        const boost::urls::url url{ "/test/path?key1=value1&key2=value2" };
        const auto parameter = std::make_shared<celeritas::http_handle_parameter>(path, url.params(), nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_param("key1").value_or(""), "value1");
        BOOST_CHECK_EQUAL(parameter->get_param("key2").value_or(""), "value2");
        BOOST_CHECK(!parameter->get_param("key3").has_value());
    }

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_with_mocks)
    {
        const auto session = std::make_shared<celeritas::mock_framework_session>();
        const auto resource_loader = std::make_shared<celeritas::mock_framework_resource_loader>();
        const auto application_loader = std::make_shared<celeritas::mock_framework_application_loader>();

        const auto parameter = std::make_shared<celeritas::http_handle_parameter>("/test", "", session, resource_loader, application_loader);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_any_io_executor(); }(), celeritas::celeritas_error);

        BOOST_CHECK_EQUAL(parameter->get_server_type(), "mock_server");

        BOOST_CHECK(!application_loader->get_task_submitted());
        parameter->submit_task([] {
        });
        BOOST_CHECK(application_loader->get_task_submitted());
    }

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_null_dependency_throws)
    {
        const auto parameter = std::make_shared<celeritas::http_handle_parameter>("", "", nullptr, nullptr, nullptr);

        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_any_io_executor(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_app_config(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_server_type(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&parameter] { std::ignore = parameter->get_database_config("db"); }(), celeritas::celeritas_error);

        BOOST_CHECK_THROW([&parameter] { parameter->submit_task([]{}); }(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
