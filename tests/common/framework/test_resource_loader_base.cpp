#include "mock/mock_framework_resource_loader.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(resource_loader_base_suite)

    BOOST_AUTO_TEST_CASE(test_mock_resource_loader_instantiation)
    {
        const auto loader = std::make_shared<celeritas::mock_framework_resource_loader>();
        BOOST_CHECK_EQUAL(loader->get_server_type(), "mock_server");
    }

BOOST_AUTO_TEST_SUITE_END()
