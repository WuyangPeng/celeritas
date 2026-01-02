#pragma once

#include "common/worker_pool/worker_pool.h"

#include <future>

namespace celeritas
{
    class multiple_producers_fixture
    {
    public:
        using class_type = multiple_producers_fixture;
        using task_type = std::function<void()>;

        static constexpr auto tasks_per_producer = 100;
        static constexpr auto total_tasks = tasks_per_producer * 2;

        multiple_producers_fixture();

        void produce();

        [[nodiscard]] int get_counter();

        [[nodiscard]] bool pop(task_type& task);

    private:
        void produce_tasks();

        void consume_tasks();

        thread_safe_queue queue_;
        std::atomic_int counter_;
        std::thread thread1_;
        std::thread thread2_;
    };
}