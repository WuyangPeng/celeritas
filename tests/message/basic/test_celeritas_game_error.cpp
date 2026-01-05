#include "message/basic/celeritas_game_error.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(celeritas_game_error_suite)

    BOOST_AUTO_TEST_CASE(test_celeritas_game_error_constructor)
    {
        const celeritas::celeritas_game_error error{ celeritas::game_error_type::token_error };

        BOOST_CHECK(error.what() != nullptr);
    }

    BOOST_AUTO_TEST_CASE(test_celeritas_game_error_constructor_with_message)
    {
        const std::string message{ "Custom error message" };
        const celeritas::celeritas_game_error error{ celeritas::game_error_type::server_error, message };

        BOOST_CHECK(error.what() != nullptr);

        const std::string what_message{ error.what() };
        BOOST_CHECK(what_message.find(message) != std::string::npos);
    }

BOOST_AUTO_TEST_SUITE_END()
