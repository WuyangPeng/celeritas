#include "message/basic/http_response.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(http_response_suite)

    BOOST_AUTO_TEST_CASE(test_http_response_default_constructor)
    {
        const celeritas::http_response response{};
        BOOST_CHECK(response.get_code() == celeritas::game_error_type::unknown);
        BOOST_CHECK(response.get_message().empty());
    }

    BOOST_AUTO_TEST_CASE(test_http_response_code_constructor)
    {
        const celeritas::http_response response{ celeritas::game_error_type::success };
        BOOST_CHECK(response.get_code() == celeritas::game_error_type::success);
        BOOST_CHECK(!response.get_message().empty());
    }

    BOOST_AUTO_TEST_CASE(test_http_response_code_message_constructor)
    {
        const std::string message{ "Operation successful" };
        const celeritas::http_response response{ celeritas::game_error_type::success, message };
        BOOST_CHECK(response.get_code() == celeritas::game_error_type::success);
        BOOST_CHECK_EQUAL(response.get_message(), message);
    }

    BOOST_AUTO_TEST_CASE(test_http_response_setters)
    {
        celeritas::http_response response{};
        response.set_code(celeritas::game_error_type::server_error);
        response.set_message("Internal Server Error");

        BOOST_CHECK(response.get_code() == celeritas::game_error_type::server_error);
        BOOST_CHECK_EQUAL(response.get_message(), "Internal Server Error");
    }

    BOOST_AUTO_TEST_CASE(test_http_response_json_serialization)
    {
        const celeritas::http_response response{ celeritas::game_error_type::invalid_parameter, "Invalid param" };
        const auto json_string = response.to_json_string();

        BOOST_CHECK(!json_string.empty());

        // 简单检查 JSON 字符串是否包含预期的字段
        BOOST_CHECK(json_string.find("\"code\"") != std::string::npos);
        BOOST_CHECK(json_string.find("\"message\"") != std::string::npos);
        BOOST_CHECK(json_string.find("Invalid param") != std::string::npos);
    }

    BOOST_AUTO_TEST_CASE(test_http_response_json_deserialization)
    {
        const std::string json_string{ R"({"code":1,"message":"Success"})" };
        const auto response = celeritas::http_response::from_json_string(json_string);

        BOOST_CHECK(response.get_code() == celeritas::game_error_type::success);
        BOOST_CHECK_EQUAL(response.get_message(), "Success");
    }

BOOST_AUTO_TEST_SUITE_END()
