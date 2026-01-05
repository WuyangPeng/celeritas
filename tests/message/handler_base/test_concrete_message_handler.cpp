#include "common/core/celeritas_error.h"
#include "message/handler_base/concrete_message_handler.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "mock/mock_payload_handler.h"

#include <boost/test/unit_test.hpp>
#include <google/protobuf/wrappers.pb.h>

BOOST_AUTO_TEST_SUITE(concrete_message_handler_suite)

    BOOST_AUTO_TEST_CASE(test_concrete_message_handler_type_name)
    {
        const celeritas::mock_payload_handler handler{};
        BOOST_CHECK_EQUAL(handler.get_supported_type_name(), celeritas::test_message_type::descriptor()->full_name());
    }

    BOOST_AUTO_TEST_CASE(test_concrete_message_handler_handle_concrete)
    {
        celeritas::mock_payload_handler handler{};
        const celeritas::test_message_type message{};

        BOOST_CHECK(!handler.is_concrete_handle_called());
        BOOST_CHECK(handler.handle(nullptr, message, {}));
        BOOST_CHECK(handler.is_concrete_handle_called());
    }

    BOOST_AUTO_TEST_CASE(test_concrete_message_handler_function_management)
    {
        celeritas::mock_payload_handler handler{};
        constexpr auto test_payload_case = 1;

        BOOST_CHECK(!handler.has_handler_function(test_payload_case));

        auto get_handler_func = [&handler] {
            std::ignore = handler.get_handler_function(test_payload_case);
        };

        BOOST_CHECK_THROW(get_handler_func(), celeritas::celeritas_error);

        handler.add_handler_function(test_payload_case, &celeritas::mock_payload_handler::handler);

        BOOST_CHECK(handler.has_handler_function(test_payload_case));
        BOOST_CHECK_NO_THROW(get_handler_func());
        BOOST_CHECK_EQUAL(handler.get_handler_function(test_payload_case), &celeritas::mock_payload_handler::handler);
    }

BOOST_AUTO_TEST_SUITE_END()
