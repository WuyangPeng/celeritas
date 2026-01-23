#include "common/buffer/buffer_guard.h"
#include "common/framework/session.h"
#include "mock/mock_network_message_callback.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(network_message_callback_suite)

    BOOST_AUTO_TEST_CASE(test_constructor)
    {
        celeritas::mock_network_message_callback callback{};

        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END()
