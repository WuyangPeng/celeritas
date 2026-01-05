#include "common/core/enum_cast.h"
#include "message/basic/header.h"
#include "proto/common/common.pb.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(header_suite)

    BOOST_AUTO_TEST_CASE(test_header_default_constructor)
    {
        constexpr celeritas::header header{};
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_empty());
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_code_constructor)
    {
        const celeritas::header header{ 100, celeritas::game_error_type::success };
        BOOST_CHECK_EQUAL(header.get_rpc(), 100);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
        BOOST_CHECK_EQUAL(header_message->to_gateway().rpc(), 100);
        BOOST_CHECK_EQUAL(header_message->to_gateway().code(), celeritas::enum_cast_underlying(celeritas::game_error_type::success));
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_userid_constructor)
    {
        const celeritas::header header{ 200, 123456789 };
        BOOST_CHECK_EQUAL(header.get_rpc(), 200);
        BOOST_CHECK_EQUAL(header.get_user_id(), 123456789);

        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_userid_constructor_check_code)
    {
        const celeritas::header header{ 200, 123456789 };
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_userid_code_constructor)
    {
        const celeritas::header header{ 300, 987654321, celeritas::game_error_type::server_error };
        BOOST_CHECK_EQUAL(header.get_rpc(), 300);
        BOOST_CHECK_EQUAL(header.get_user_id(), 987654321);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::server_error);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
    }

    BOOST_AUTO_TEST_CASE(test_header_userid_constructor)
    {
        const celeritas::header header{ 111222333 };
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), 111222333);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_gateway_message)
    {
        const celeritas::header header{ 123, 456, celeritas::game_error_type::unknown };
        BOOST_CHECK_EQUAL(header.get_rpc(), 123);
        BOOST_CHECK_EQUAL(header.get_user_id(), 456);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_gateway());
        BOOST_CHECK_EQUAL(header_message->gateway().user_id(), 456);
        BOOST_CHECK_EQUAL(header_message->gateway().rpc(), 123);
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_server_message)
    {
        const celeritas::header header{ 0, 456, celeritas::game_error_type::unknown };

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_server());
        BOOST_CHECK_EQUAL(header_message->server().user_id(), 456);
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_client_message)
    {
        const celeritas::header header{ 123, 0, celeritas::game_error_type::unknown };

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_client());
        BOOST_CHECK_EQUAL(header_message->client().rpc(), 123);
    }

    BOOST_AUTO_TEST_CASE(test_header_empty_message_constructor)
    {
        const celeritas::header::empty_message_header_type empty_header{};

        const celeritas::header header{ empty_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_empty());
    }

    BOOST_AUTO_TEST_CASE(test_header_server_message_constructor)
    {
        celeritas::header::server_message_header_type server_header{};
        server_header.set_user_id(666);

        const celeritas::header header{ server_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), 666);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_client_message_constructor)
    {
        celeritas::header::client_message_header_type client_header{};
        client_header.set_rpc(600);

        const celeritas::header header{ client_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 600);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_gateway_message_constructor)
    {
        celeritas::header::gateway_message_header_type gateway_header{};
        gateway_header.set_rpc(700);
        gateway_header.set_user_id(888);

        const celeritas::header header{ gateway_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 700);
        BOOST_CHECK_EQUAL(header.get_user_id(), 888);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_to_gateway_message_constructor)
    {
        celeritas::header::to_gateway_message_header_type to_gateway_header{};
        to_gateway_header.set_rpc(800);
        to_gateway_header.set_user_id(999);
        to_gateway_header.set_code(static_cast<int32_t>(celeritas::game_error_type::success));

        const celeritas::header header{ to_gateway_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 800);
        BOOST_CHECK_EQUAL(header.get_user_id(), 999);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);
    }

BOOST_AUTO_TEST_SUITE_END()
