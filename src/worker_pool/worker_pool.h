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

        // 创建指定数量的工作线程
        explicit worker_pool(size_t num_threads);
        ~worker_pool() noexcept;
        worker_pool(const worker_pool& rhs) = delete;
        worker_pool& operator=(const worker_pool& rhs) = delete;
        worker_pool(worker_pool&& rhs) noexcept;
        worker_pool& operator=(worker_pool&& rhs) noexcept;

        // 提交任务到线程池
        void submit(std::function<void()> task);

    private:
        thread_safe_queue queue_;
        std::vector<std::jthread> workers_;
    };
}
