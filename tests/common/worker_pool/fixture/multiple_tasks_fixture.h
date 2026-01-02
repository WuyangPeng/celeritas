#pragma once

#include "common/worker_pool/worker_pool.h"

#include <future>

namespace celeritas
{
    class multiple_tasks_fixture
    {
    public:
        using class_type = multiple_tasks_fixture;

        static constexpr auto num_tasks = 10;

        multiple_tasks_fixture();

        void submit_tasks();

        void wait_for_tasks();

        [[nodiscard]] int get_counter();

    private:
        using future_type = std::vector<std::future<void> >;

        void do_submit_tasks();

        worker_pool pool_;
        std::atomic_int counter_;
        future_type futures_;
    };
}