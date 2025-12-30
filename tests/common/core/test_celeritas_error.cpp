#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(celeritas_error_suite)

    BOOST_AUTO_TEST_CASE(test_string_constructor)
    {
        const std::string errorMessage{ "This is a test error message." };

        try
        {
            throw celeritas::celeritas_error{ errorMessage };
        }
        catch (const celeritas::celeritas_error& error)
        {
            BOOST_CHECK_EQUAL(error.what(), errorMessage);
        }
    }

    BOOST_AUTO_TEST_CASE(test_format_constructor)
    {
        const std::string expectedMessage{ "This is a test with number 42 and string value." };

        try
        {
            throw celeritas::celeritas_error{ "This is a test with number {} and {} value.", 42, "string" };
        }
        catch (const celeritas::celeritas_error& error)
        {
            BOOST_CHECK_EQUAL(error.what(), expectedMessage);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
