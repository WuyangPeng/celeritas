#include "common/worker_pool/worker_pool.h"
#include "fixture/multiple_tasks_fixture.h"

#include <boost/test/unit_test.hpp>

#include <future>
#include <memory>
#include <vector>

BOOST_AUTO_TEST_SUITE(worker_pool_suite)

    BOOST_AUTO_TEST_CASE(test_submit_task)
    {
        celeritas::worker_pool pool{ 2 };
        std::promise<int> promise{};
        auto future = promise.get_future();

        pool.submit([&promise] {
            promise.set_value(42);
        });

        BOOST_CHECK_EQUAL(future.get(), 42);
    }

    BOOST_FIXTURE_TEST_CASE(test_multiple_tasks, celeritas::multiple_tasks_fixture)
    {
        submit_tasks();
        wait_for_tasks();

        BOOST_CHECK_EQUAL(get_counter(), num_tasks);
    }

BOOST_AUTO_TEST_SUITE_END()