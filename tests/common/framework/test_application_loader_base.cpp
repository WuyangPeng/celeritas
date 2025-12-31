#include "common/framework/application_loader_base.h"
#include "mock/mock_application_loader.h"

#include <boost/test/unit_test.hpp>

#include <atomic>

BOOST_AUTO_TEST_SUITE(application_loader_base_suite)

    BOOST_AUTO_TEST_CASE(test_submit_task)
    {
        const auto loader = std::make_shared<celeritas::mock_application_loader>();
        std::atomic task_executed{ false };

        loader->submit_task([&task_executed] {
            task_executed = true;
        });

        BOOST_CHECK(loader->get_task_submitted());
        BOOST_CHECK(task_executed);
    }

BOOST_AUTO_TEST_SUITE_END()
