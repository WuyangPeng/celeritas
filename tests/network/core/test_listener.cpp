#include "fixture/listener_fixture.h"
#include "message/basic/header.h"
#include "mock/mock_session_base.h"
#include "network/core/listener.h"
#include "network/session_helper/session_callback.h"

#include <google/protobuf/empty.pb.h>

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(listener_suite)

    BOOST_FIXTURE_TEST_CASE(test_constructor, celeritas::listener_fixture)
    {
        BOOST_CHECK_EQUAL(get_listener()->test_get_game_server_id(), get_game_server_id());
        BOOST_CHECK(get_listener()->test_get_network_message_callback().expired());
        BOOST_CHECK(get_listener()->get_server_network_type() == get_server_type());
    }

    BOOST_FIXTURE_TEST_CASE(test_start_calls_accept_connections, celeritas::listener_fixture)
    {
        get_listener()->start();
        get_io_context().run();

        BOOST_CHECK(get_listener()->accept_connections_called());
    }

    BOOST_FIXTURE_TEST_CASE(test_stop, celeritas::listener_fixture)
    {
        get_listener()->stop();
        BOOST_CHECK(get_listener()->stop_called());
    }

    BOOST_FIXTURE_TEST_CASE(test_get_session, celeritas::listener_fixture)
    {
        const celeritas::session_callback session_callback{ std::weak_ptr<celeritas::listener_sessions_base>(),
                                                            std::weak_ptr<celeritas::network_message_callback>() };
        const auto mock_session = std::make_shared<celeritas::mock_session_base>(get_io_context().get_executor(), get_server_type(), 1, session_callback);
        get_listener()->set_session_to_return(mock_session);

        const auto session = get_listener()->get_session(1);
        BOOST_CHECK_EQUAL(session, mock_session);
    }

    BOOST_FIXTURE_TEST_CASE(test_write, celeritas::listener_fixture)
    {
        const std::string server_type_str = "test_type";
        const std::string instance_id = "test_instance";
        const celeritas::header header{ 123 };

        const google::protobuf::Empty request{};

        const auto result = get_listener()->write(server_type_str, instance_id, header, request);

        BOOST_CHECK(result);
        BOOST_CHECK(get_listener()->write_called());
        BOOST_CHECK_EQUAL(get_listener()->get_write_server_type(), server_type_str);
        BOOST_CHECK_EQUAL(get_listener()->get_write_instance_id(), instance_id);
        BOOST_CHECK_EQUAL(get_listener()->get_write_header().get_user_id(), header.get_user_id());
        BOOST_CHECK(get_listener()->get_write_request() == &request);
    }

BOOST_AUTO_TEST_SUITE_END()
