#include "initializer/initializer_constant.h"
#include "message/handler_base/mock/mock_protobuf_message_handler.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "message/registry/protobuf_message_registry.h"

#include <boost/test/unit_test.hpp>
#include <google/protobuf/any.pb.h>

BOOST_AUTO_TEST_SUITE(protobuf_message_registry_suite)

    BOOST_AUTO_TEST_CASE(test_protobuf_message_registry_register_and_dispatch)
    {
        const auto registry = std::make_shared<celeritas::protobuf_message_registry>(celeritas::auth_type);
        const auto handler = std::make_shared<celeritas::mock_protobuf_message_handler>();
        const auto handle_parameter = std::make_shared<celeritas::protobuf_handle_parameter>(celeritas::header{}, nullptr, nullptr, nullptr, nullptr);
        const google::protobuf::Any any{};

        registry->register_handler(handler);

        BOOST_CHECK(!handler->is_was_handled());
        BOOST_CHECK(registry->dispatch(handle_parameter, any));
        BOOST_CHECK(handler->is_was_handled());
    }

    BOOST_AUTO_TEST_CASE(test_protobuf_message_registry_dispatch_not_found)
    {
        const auto registry = std::make_shared<celeritas::protobuf_message_registry>(celeritas::auth_type);
        const auto handle_parameter = std::make_shared<celeritas::protobuf_handle_parameter>(celeritas::header{}, nullptr, nullptr, nullptr, nullptr);
        const google::protobuf::Any any{};

        BOOST_CHECK(!registry->dispatch(handle_parameter, any));
    }

BOOST_AUTO_TEST_SUITE_END()
