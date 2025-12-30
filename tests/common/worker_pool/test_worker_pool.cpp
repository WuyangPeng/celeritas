#include "common/worker_pool/worker_pool.h"

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <future>
#include <memory>

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

    BOOST_AUTO_TEST_CASE(test_multiple_tasks)
    {
        celeritas::worker_pool pool{ 4 };
        std::atomic counter{ 0 };

        std::vector<std::future<void> > futures{};

        for (auto i = 0; i < 10; ++i)
        {
            auto task = std::make_shared<std::packaged_task<void()> >([&counter] {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++counter;
            });
            futures.push_back(task->get_future());
            pool.submit([task] {
                (*task)();
            });
        }

        for (auto& future : futures)
        {
            future.get();
        }

        BOOST_CHECK_EQUAL(counter.load(), 10);
    }

BOOST_AUTO_TEST_SUITE_END()
