#include "message/handler_base/http_base_message_handler.h"
#include "mock/mock_http_message_handler.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(http_base_message_handler_suite)

    BOOST_AUTO_TEST_CASE(test_http_base_message_handler_creation_and_handle)
    {
        celeritas::mock_http_message_handler handler{};

        BOOST_CHECK_EQUAL(handler.get_supported_type_name(), "test_handler");
        BOOST_CHECK(!handler.is_was_handled());
        const auto result = handler.handle(nullptr, {});
        BOOST_CHECK(result);
        BOOST_CHECK(handler.is_was_handled());
    }

BOOST_AUTO_TEST_SUITE_END()
