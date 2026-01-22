#include "config/basic/server_network_type.h"
#include "message/basic/header.h"
#include "mock/mock_listener.h"
#include "network/core/listener.h"
#include "network/core/network_message_callback.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(listener_tests)

    BOOST_AUTO_TEST_CASE(constructor)
    {
        boost::asio::io_context io_context{};
        const std::weak_ptr<celeritas::network_message_callback> callback{};
        const std::string game_server_id{ "test_server" };
        constexpr auto server_type = celeritas::server_network_type::tcp;

        celeritas::mock_listener listener{ io_context.get_executor(), callback, game_server_id, server_type };

        BOOST_CHECK_EQUAL(listener.test_get_game_server_id(), game_server_id);
        BOOST_CHECK(listener.test_get_network_message_callback().expired());
        BOOST_CHECK(listener.get_server_network_type() == server_type);
    }

BOOST_AUTO_TEST_SUITE_END()
