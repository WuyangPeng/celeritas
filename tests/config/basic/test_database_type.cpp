#include "common/core/celeritas_error.h"
#include "config/basic/database_type.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_type_suite)

    BOOST_AUTO_TEST_CASE(test_get_database_type_valid)
    {
        BOOST_CHECK(celeritas::get_database_type("mysql") == celeritas::database_type::mysql);
        BOOST_CHECK(celeritas::get_database_type("mongo") == celeritas::database_type::mongo);
        BOOST_CHECK(celeritas::get_database_type("redis") == celeritas::database_type::redis);
    }

    BOOST_AUTO_TEST_CASE(test_get_database_type_invalid)
    {
        BOOST_CHECK_THROW([] { std::ignore = celeritas::get_database_type("unknown"); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([] { std::ignore =celeritas::get_database_type(""); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([] { std::ignore =celeritas::get_database_type("postgresql"); }(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
