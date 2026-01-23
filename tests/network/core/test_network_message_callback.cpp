#include "common/buffer/buffer_guard.h"
#include "fixture/network_message_callback_fixture.h"
#include "mock/mock_network_message_callback.h"
#include "mock/mock_session_base.h"
#include "network/core/message_header.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/url/url_view.hpp>

BOOST_AUTO_TEST_SUITE(network_message_callback_suite)

    BOOST_FIXTURE_TEST_CASE(test_message_header_callback, celeritas::network_message_callback_fixture)
    {
        celeritas::mock_network_message_callback callback{};
        const celeritas::message_header header{ 1, 2 };
        celeritas::buffer_guard buffer{ std::move(move_to_buffer_pool_data()), 0 };

        callback.call_back(header, std::move(buffer), get_session());

        BOOST_CHECK(callback.was_message_header_called());
        BOOST_CHECK_EQUAL(callback.get_last_message_header().get_header_size(), 1);
        BOOST_CHECK_EQUAL(callback.get_last_message_header().get_body_size(), 2);
    }

    BOOST_FIXTURE_TEST_CASE(test_url_params_callback, celeritas::network_message_callback_fixture)
    {
        celeritas::mock_network_message_callback callback{};
        const std::string path{ "/test" };
        const boost::urls::url_view url_view{ "/test?a=1&b=2" };

        callback.call_back(path, url_view.params(), get_session());

        BOOST_CHECK(callback.was_url_params_called());
        BOOST_CHECK_EQUAL(callback.get_last_path(), path);
    }

    BOOST_FIXTURE_TEST_CASE(test_string_params_callback, celeritas::network_message_callback_fixture)
    {
        celeritas::mock_network_message_callback callback{};
        const std::string path = "/test";
        const std::string params = "a=1&b=2";

        callback.call_back(path, params, get_session());

        BOOST_CHECK(callback.was_string_params_called());
        BOOST_CHECK_EQUAL(callback.get_last_path(), path);
        BOOST_CHECK_EQUAL(callback.get_last_string_params(), params);
    }

    BOOST_AUTO_TEST_CASE(test_send_offline_message)
    {
        celeritas::mock_network_message_callback callback{};
        constexpr int64_t session_id{ 12345 };

        callback.send_offline_message(session_id);

        BOOST_CHECK(callback.was_offline_called());
        BOOST_CHECK_EQUAL(callback.get_last_session_id(), session_id);
    }

BOOST_AUTO_TEST_SUITE_END()
