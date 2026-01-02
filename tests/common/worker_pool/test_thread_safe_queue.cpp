#include "common/worker_pool/thread_safe_queue.h"
#include "fixture/multiple_producers_fixture.h"

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

namespace
{
    constexpr auto num_tasks = 200;
    constexpr auto num_consumers = 5;

    // 准备任务：向队列预先填充任务
    void prepare_tasks(celeritas::thread_safe_queue& queue, std::atomic<int>& counter)
    {
        for (auto i = 0; i < num_tasks; ++i)
        {
            queue.push([&counter] {
                ++counter;
            });
        }
    }

    // 消费者线程函数
    void consumer_thread_func(celeritas::thread_safe_queue& queue, std::atomic<int>& consumed_count)
    {
        celeritas::thread_safe_queue::task_type task{};
        while (queue.pop(task))
        {
            task();
            ++consumed_count;
        }
    }
}

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

    BOOST_AUTO_TEST_CASE(test_multiple_consumers)
    {
        celeritas::thread_safe_queue queue{};
        std::atomic counter{ 0 };

        prepare_tasks(queue, counter);

        std::atomic consumed_count{ 0 };
        std::vector<std::thread> consumers{};

        for (auto i = 0; i < num_consumers; ++i)
        {
            consumers.emplace_back(consumer_thread_func, std::ref(queue), std::ref(consumed_count));
        }

        while (consumed_count < num_tasks)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        queue.stop();

        for (auto& t : consumers)
        {
            t.join();
        }

        BOOST_CHECK_EQUAL(counter, num_tasks);
        BOOST_CHECK_EQUAL(consumed_count, num_tasks);
    }

BOOST_AUTO_TEST_SUITE_END()