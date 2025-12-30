#include "common/worker_pool/thread_safe_queue.h"

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

    BOOST_AUTO_TEST_CASE(test_multiple_producers)
    {
        celeritas::thread_safe_queue queue{};
        std::atomic counter{ 0 };

        auto produce = [&] {
            for (auto i = 0; i < 100; ++i)
            {
                queue.push([&counter] {
                    ++counter;
                });
            }
        };

        std::thread t1{ produce };
        std::thread t2{ produce };

        t1.join();
        t2.join();

        auto executed_tasks = 0;
        celeritas::thread_safe_queue::task_type task{};
        while (executed_tasks < 200 && queue.pop(task))
        {
            task();
            ++executed_tasks;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        BOOST_CHECK(!queue.pop(task));

        BOOST_CHECK_EQUAL(counter, 200);
    }

    BOOST_AUTO_TEST_CASE(test_multiple_consumers)
    {
        celeritas::thread_safe_queue queue{};
        std::atomic counter{ 0 };

        for (auto i = 0; i < 200; ++i)
        {
            queue.push([&counter] {
                ++counter;
            });
        }

        std::atomic consumed_count{ 0 };
        auto consume = [&] {
            celeritas::thread_safe_queue::task_type task{};
            while (queue.pop(task))
            {
                task();
                ++consumed_count;
            }
        };

        std::vector<std::thread> consumers{};
        for (auto i = 0; i < 5; ++i)
        {
            consumers.emplace_back(consume);
        }

        while (consumed_count < 200)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        queue.stop();

        for (auto& t : consumers)
        {
            t.join();
        }

        BOOST_CHECK_EQUAL(counter, 200);
        BOOST_CHECK_EQUAL(consumed_count, 200);
    }

BOOST_AUTO_TEST_SUITE_END()
