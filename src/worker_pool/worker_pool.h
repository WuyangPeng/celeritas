#pragma once

#include "thread_safe_queue.h"

#include <functional>
#include <queue>
#include <thread>

namespace celeritas
{
    class worker_pool
    {
    public:
        using class_type = worker_pool;
        using task_type = thread_safe_queue::task_type;

        // 创建指定数量的工作线程
        explicit worker_pool(size_t num_threads);
        ~worker_pool() noexcept;
        worker_pool(const worker_pool& rhs) = delete;
        worker_pool& operator=(const worker_pool& rhs) = delete;
        worker_pool(worker_pool&& rhs) noexcept = delete;
        worker_pool& operator=(worker_pool&& rhs) noexcept = delete;

        // 提交任务到线程池
        void submit(task_type task);

    private:
        using worker_type = std::vector<std::jthread>;

        static void execute_task(const task_type& task);

        thread_safe_queue queue_;
        worker_type workers_;
    };
}
