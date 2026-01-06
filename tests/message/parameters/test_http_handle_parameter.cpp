#include "common/core/celeritas_error.h"
#include "config/aggregate/app_config.h"
#include "fixture/http_handle_parameter_fixture.h"
#include "message/parameters/http_handle_parameter.h"
#include "mock/mock_parameters_application_loader.h"
#include "mock/mock_parameters_resource_loader.h"
#include "mock/mock_parameters_session.h"
#include "service_registry/data/health_check_level_type.h"

#include <boost/url.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    const std::string path{ "/test/path" };

    void test_http_handle_parameter(const celeritas::http_handle_parameter& http_handle_parameter)
    {
        BOOST_CHECK_EQUAL(http_handle_parameter.get_path(), path);
        BOOST_CHECK_EQUAL(http_handle_parameter.get_param("key").value_or(""), "value");
        BOOST_CHECK_NO_THROW([&http_handle_parameter] { std::ignore = http_handle_parameter.get_app_config(); }());
    }
}

BOOST_AUTO_TEST_SUITE(http_handle_parameter_suite)

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_creation_and_getters)
    {
        const std::string params{ "key=value" };
        const auto parameter = std::make_shared<celeritas::http_handle_parameter>(path, params, nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_path(), path);
        BOOST_CHECK_EQUAL(parameter->get_response(), params);
    }

    BOOST_AUTO_TEST_CASE(test_http_handle_parameter_get_param)
    {
        const boost::urls::url url{ "/test/path?key1=value1&key2=value2" };
        const auto parameter = std::make_shared<celeritas::http_handle_parameter>(path, url.params(), nullptr, nullptr, nullptr);

        BOOST_CHECK_EQUAL(parameter->get_param("key1").value_or(""), "value1");
        BOOST_CHECK_EQUAL(parameter->get_param("key2").value_or(""), "value2");
        BOOST_CHECK(!parameter->get_param("key3").has_value());
    }

    BOOST_FIXTURE_TEST_CASE(test_http_handle_parameter_constructors, celeritas::http_handle_parameter_fixture)
    {
        const boost::urls::url url{ "/test/path?key=value" };

        celeritas::http_handle_parameter original{ path, url.params(), get_session(), get_resource_loader(), get_application_loader() };
        test_http_handle_parameter(original);

        auto copy = original;
        test_http_handle_parameter(copy);

        auto moved = std::move(copy);
        test_http_handle_parameter(moved);
    }

    BOOST_FIXTURE_TEST_CASE(test_http_handle_parameter_with_mocks, celeritas::http_handle_parameter_fixture)
    {
        BOOST_CHECK_NO_THROW([this] { std::ignore = get_parameter()->get_any_io_executor(); }());
        BOOST_CHECK(get_parameter()->get_any_io_executor().target<boost::asio::io_context::executor_type>());

        BOOST_CHECK_EQUAL(get_parameter()->get_server_type(), "mock_server");

        boost::asio::co_spawn(
            get_io_context(),
            [this]() -> boost::asio::awaitable<void> {
                const auto health_check_level = co_await get_parameter()->get_health_check_level();
                BOOST_CHECK(health_check_level == celeritas::health_check_level_type::health);
            },
            boost::asio::detached);

        get_io_context().run();

        BOOST_CHECK(!get_application_loader()->get_task_submitted());
        get_parameter()->submit_task([] {
        });
        BOOST_CHECK(get_application_loader()->get_task_submitted());

        const auto app_config = get_parameter()->get_app_config();
        BOOST_CHECK_EQUAL(app_config->get_external_host(), "192.168.1.100");

        const auto database_config = get_parameter()->get_database_config("test_db");
        BOOST_CHECK(database_config->get_database_type() == celeritas::database_type::mysql);
        BOOST_CHECK_EQUAL(database_config->get_name(), "test_db");
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

    BOOST_FIXTURE_TEST_CASE(test_http_handle_parameter_write_immediately, celeritas::http_handle_parameter_fixture)
    {
        boost::asio::co_spawn(
            get_io_context(),
            [this]() -> boost::asio::awaitable<void> {
                co_await get_parameter()->write_immediately("response");
            },
            boost::asio::detached);

        get_io_context().run();

        BOOST_CHECK(get_session()->is_write_immediately_call());
        BOOST_CHECK(get_session()->is_remove_session_call());
    }

BOOST_AUTO_TEST_SUITE_END()
