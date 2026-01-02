#include "common/worker_pool/thread_safe_queue.h"
#include "fixture/multiple_consumers_fixture.h"
#include "fixture/multiple_producers_fixture.h"

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

BOOST_AUTO_TEST_SUITE(thread_safe_queue_suite)

    BOOST_AUTO_TEST_CASE(test_push_and_pop)
    {
        celeritas::thread_safe_queue queue{};
        std::atomic value{ 0 };

        queue.push([&value] {
            value = 1;
        });

        celeritas::thread_safe_queue::task_type task{};
        BOOST_CHECK(queue.pop(task));

        task();
        BOOST_CHECK_EQUAL(value, 1);
    }

    BOOST_AUTO_TEST_CASE(test_stop_queue)
    {
        celeritas::thread_safe_queue queue{};
        std::atomic task_executed{ false };

        queue.push([&task_executed] {
            task_executed = true;
        });
        queue.stop();

        celeritas::thread_safe_queue::task_type task{};
        BOOST_CHECK(queue.pop(task));
        BOOST_CHECK(!task_executed);
    }

    BOOST_FIXTURE_TEST_CASE(test_multiple_producers, celeritas::multiple_producers_fixture)
    {
        produce();

        celeritas::thread_safe_queue::task_type task{};
        BOOST_CHECK(!pop(task));

        BOOST_CHECK_EQUAL(get_counter(), total_tasks);
    }

    BOOST_FIXTURE_TEST_CASE(test_multiple_consumers, celeritas::multiple_consumers_fixture)
    {
        prepare_tasks();
        run_consumers();

        BOOST_CHECK_EQUAL(get_counter(), num_tasks);
        BOOST_CHECK_EQUAL(get_consumed_count(), num_tasks);
    }

BOOST_AUTO_TEST_SUITE_END()