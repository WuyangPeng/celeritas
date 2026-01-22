#include "config/basic/server_network_type.h"
#include "mock/mock_session_base.h"
#include "network/core/listener.h"
#include "network/core/network_message_callback.h"
#include "network/core/session_base.h"
#include "network/session_helper/session_callback.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(session_base_tests)

    BOOST_AUTO_TEST_CASE(constructor_and_getters)
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

    BOOST_AUTO_TEST_CASE(instance_id)
    {
        boost::asio::io_context io_context{};

        const celeritas::session_callback callback{ std::weak_ptr<celeritas::listener_sessions_base>(),
                                                    std::weak_ptr<celeritas::network_message_callback>() };
        celeritas::mock_session_base session{ io_context.get_executor(), celeritas::server_network_type::tcp, 1, callback };
        const std::string instance_id{ "test_instance" };

        session.set_instance_id(instance_id);
        BOOST_CHECK_EQUAL(session.get_instance_id(), instance_id);
    }

BOOST_AUTO_TEST_SUITE_END()
