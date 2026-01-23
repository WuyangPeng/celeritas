#include "auth_server/auth_server.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/mock/mock_http_message_handler.h"
#include "message/parameters/http_handle_parameter.h"
#include "message/registry/http_message_registry.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(http_message_registry_suite)

    BOOST_AUTO_TEST_CASE(test_http_message_registry_register_and_dispatch)
    {
        const auto registry = std::make_shared<celeritas::http_message_registry>(celeritas::auth_type);
        const auto handler = std::make_shared<celeritas::mock_http_message_handler>();
        const auto handle_parameter = std::make_shared<celeritas::http_handle_parameter>("test_handler", "", nullptr, nullptr, nullptr);

        registry->register_handler(handler);

        BOOST_CHECK(!handler->is_was_handled());
        BOOST_CHECK(registry->dispatch(handle_parameter));
        BOOST_CHECK(handler->is_was_handled());
    }

    BOOST_AUTO_TEST_CASE(test_http_message_registry_dispatch_not_found)
    {
        const auto registry = std::make_shared<celeritas::http_message_registry>(celeritas::auth_type);
        const auto handle_parameter = std::make_shared<celeritas::http_handle_parameter>("unknown_handler", "", nullptr, nullptr, nullptr);

        BOOST_CHECK(!registry->dispatch(handle_parameter));
    }

BOOST_AUTO_TEST_SUITE_END()
