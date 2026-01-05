#include "message/basic/game_error_type.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(game_error_type_suite)

    BOOST_AUTO_TEST_CASE(test_get_game_error_description)
    {
        BOOST_CHECK(!celeritas::get_game_error_description(celeritas::game_error_type::success).empty());
        BOOST_CHECK(!celeritas::get_game_error_description(celeritas::game_error_type::unknown).empty());
        BOOST_CHECK(!celeritas::get_game_error_description(celeritas::game_error_type::server_error).empty());
    }

BOOST_AUTO_TEST_SUITE_END()
