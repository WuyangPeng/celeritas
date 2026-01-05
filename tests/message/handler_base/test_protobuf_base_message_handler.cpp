#include "mock/mock_protobuf_message_handler.h"

#include <boost/test/unit_test.hpp>
#include <google/protobuf/any.pb.h>

BOOST_AUTO_TEST_SUITE(protobuf_base_message_handler_suite)

    BOOST_AUTO_TEST_CASE(test_protobuf_base_message_handler_creation_and_handle)
    {
        celeritas::mock_protobuf_message_handler handler{};
        const google::protobuf::Any any{};

        BOOST_CHECK_EQUAL(handler.get_supported_type_name(), "google.protobuf.Any");

        BOOST_CHECK(!handler.is_was_handled());
        const auto result = handler.handle(nullptr, any, {});
        BOOST_CHECK(result);
        BOOST_CHECK(handler.is_was_handled());
    }

BOOST_AUTO_TEST_SUITE_END()
