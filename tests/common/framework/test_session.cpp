#include "common/framework/session.h"
#include "mock/mock_framework_session.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(session_suite)

    BOOST_AUTO_TEST_CASE(test_mock_session_instantiation)
    {
        const celeritas::mock_framework_session session{};
        BOOST_CHECK_EQUAL(session.get_session_id(), 123);
    }

    BOOST_AUTO_TEST_CASE(test_instance_id)
    {
        celeritas::mock_framework_session session{};
        session.set_instance_id("test_id");
        BOOST_CHECK_EQUAL(session.get_instance_id(), "test_id");
    }

BOOST_AUTO_TEST_SUITE_END()
