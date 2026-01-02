#pragma once

#include "common/worker_pool/worker_pool.h"

#include <future>

namespace celeritas
{
    class multiple_consumers_fixture
    {
    public:
        using class_type = multiple_consumers_fixture;

        static constexpr auto num_tasks = 200;
        static constexpr auto num_consumers = 5;

        void prepare_tasks();

        void run_consumers();

        [[nodiscard]] int get_counter() const;

        [[nodiscard]] int get_consumed_count() const;

    private:
        using thread_container = std::vector<std::thread>;

        void consumer_thread_func();

        thread_safe_queue queue_{};
        std::atomic_int counter_{ 0 };
        std::atomic_int consumed_count_{ 0 };
        thread_container consumers_{};
    };
}