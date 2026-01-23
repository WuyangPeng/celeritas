#include "config/basic/server_network_type.h"
#include "fixture/session_base_fixture.h"
#include "message/basic/header.h"
#include "mock/mock_session_base.h"
#include "network/core/session_base.h"
#include "network/listener/listener_sessions_base.h"
#include "proto/client/player/login.pb.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(session_base_suite)

    BOOST_AUTO_TEST_CASE(test_constructor_and_getters)
    {
        boost::asio::io_context io_context{};
        constexpr auto server_type = celeritas::server_network_type::tcp;
        constexpr int64_t session_id{ 12345 };

        const celeritas::session_callback callback{ std::weak_ptr<celeritas::listener_sessions_base>(),
                                                    std::weak_ptr<celeritas::network_message_callback>() };

        const celeritas::mock_session_base session{ io_context.get_executor(), server_type, session_id, callback };

        BOOST_CHECK_EQUAL(session.get_session_id(), session_id);
        BOOST_CHECK(session.get_server_network_type() == server_type);
    }

    BOOST_AUTO_TEST_CASE(test_instance_id)
    {
        boost::asio::io_context io_context{};

        const celeritas::session_callback callback{ std::weak_ptr<celeritas::listener_sessions_base>(),
                                                    std::weak_ptr<celeritas::network_message_callback>() };
        celeritas::mock_session_base session{ io_context.get_executor(), celeritas::server_network_type::tcp, 1, callback };
        const std::string instance_id{ "test_instance" };

        session.set_instance_id(instance_id);
        BOOST_CHECK_EQUAL(session.get_instance_id(), instance_id);
    }

    BOOST_FIXTURE_TEST_CASE(test_write, celeritas::session_base_fixture)
    {
        constexpr celeritas::header header{};
        celeritas::proto::client::login_request request{};
        request.set_token("test");

        get_session()->write(header, request);

        BOOST_CHECK(get_session()->was_do_write_called());
        BOOST_CHECK_GT(get_session()->get_last_write_size(), 0);
        set_test_end(true);
    }

    BOOST_FIXTURE_TEST_CASE(test_write_immediately, celeritas::session_base_fixture)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const std::string data{ "test data" };
            co_await get_session()->write_immediately(data);

            BOOST_CHECK(get_session()->was_do_write_immediately_called());
            BOOST_CHECK_EQUAL(get_session()->get_last_write_size(), data.size());

            set_test_end(true);
        });
    }

    BOOST_FIXTURE_TEST_CASE(test_remove_session, celeritas::session_base_fixture)
    {
        get_session()->remove_session();

        BOOST_CHECK(get_listener_sessions()->is_remove_session_called());
        BOOST_CHECK_EQUAL(get_listener_sessions()->get_last_removed_session_id(), get_session()->get_session_id());

        set_test_end(true);
    }

BOOST_AUTO_TEST_SUITE_END()
