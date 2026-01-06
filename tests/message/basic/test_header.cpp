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
        constexpr auto rpc = 100;

        const celeritas::header header{ rpc, celeritas::game_error_type::success };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
        BOOST_CHECK_EQUAL(header_message->to_gateway().rpc(), rpc);
        BOOST_CHECK_EQUAL(header_message->to_gateway().code(), celeritas::enum_cast_underlying(celeritas::game_error_type::success));
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_user_id_constructor)
    {
        constexpr auto rpc = 200;
        constexpr auto user_id = 123456789;

        const celeritas::header header{ rpc, user_id };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);

        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);
    }

    BOOST_AUTO_TEST_CASE(test_header_rpc_user_id_constructor_check_code)
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
        constexpr auto rpc = 300;
        constexpr auto user_id = 987654321;

        const celeritas::header header{ rpc, user_id, celeritas::game_error_type::server_error };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::server_error);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
    }

    BOOST_AUTO_TEST_CASE(test_header_userid_constructor)
    {
        constexpr auto user_id = 111222333;

        const celeritas::header header{ user_id };
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_to_gateway());
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_gateway_message)
    {
        constexpr auto rpc = 123;
        constexpr auto user_id = 456;

        const celeritas::header header{ rpc, user_id, celeritas::game_error_type::unknown };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_gateway());
        BOOST_CHECK_EQUAL(header_message->gateway().user_id(), user_id);
        BOOST_CHECK_EQUAL(header_message->gateway().rpc(), rpc);
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_server_message)
    {
        constexpr auto user_id = 456;

        const celeritas::header header{ 0, user_id, celeritas::game_error_type::unknown };

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_server());
        BOOST_CHECK_EQUAL(header_message->server().user_id(), user_id);
    }

    BOOST_AUTO_TEST_CASE(test_header_logic_client_message)
    {
        constexpr auto rpc = 123;

        const celeritas::header header{ rpc, 0, celeritas::game_error_type::unknown };

        const auto message = header.get_message();
        const auto header_message = std::dynamic_pointer_cast<celeritas::proto::common::header>(message);
        BOOST_REQUIRE(header_message);
        BOOST_CHECK(header_message->has_client());
        BOOST_CHECK_EQUAL(header_message->client().rpc(), rpc);
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
        constexpr auto user_id = 666;

        celeritas::header::server_message_header_type server_header{};
        server_header.set_user_id(user_id);

        const celeritas::header header{ server_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), 0);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_client_message_constructor)
    {
        constexpr auto rpc = 600;

        celeritas::header::client_message_header_type client_header{};
        client_header.set_rpc(rpc);

        const celeritas::header header{ client_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), 0);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_gateway_message_constructor)
    {
        constexpr auto rpc = 700;
        constexpr auto user_id = 888;

        celeritas::header::gateway_message_header_type gateway_header{};
        gateway_header.set_rpc(rpc);
        gateway_header.set_user_id(user_id);

        const celeritas::header header{ gateway_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::unknown);
    }

    BOOST_AUTO_TEST_CASE(test_header_to_gateway_message_constructor)
    {
        constexpr auto rpc = 800;
        constexpr auto user_id = 999;

        celeritas::header::to_gateway_message_header_type to_gateway_header{};
        to_gateway_header.set_rpc(rpc);
        to_gateway_header.set_user_id(user_id);
        to_gateway_header.set_code(enum_cast_underlying(celeritas::game_error_type::success));

        const celeritas::header header{ to_gateway_header };
        BOOST_CHECK_EQUAL(header.get_rpc(), rpc);
        BOOST_CHECK_EQUAL(header.get_user_id(), user_id);
        BOOST_CHECK(header.get_code() == celeritas::game_error_type::success);
    }

BOOST_AUTO_TEST_SUITE_END()
