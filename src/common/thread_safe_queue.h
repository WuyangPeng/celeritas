#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace celeritas
{
    class thread_safe_queue
    {
    public:
        using class_type = thread_safe_queue;
        using task_type = std::function<void()>;

        thread_safe_queue() noexcept = default;

        ~thread_safe_queue() noexcept = default;

        thread_safe_queue(const thread_safe_queue& rhs) = delete;

        thread_safe_queue& operator=(const thread_safe_queue& rhs) = delete;

        thread_safe_queue(thread_safe_queue&& rhs) noexcept = delete;

        thread_safe_queue& operator=(thread_safe_queue&& rhs) noexcept = delete;

        void push(task_type task);

        bool pop(task_type& task);

        void stop();

    private:
        using queue_type = std::queue<task_type>;

        queue_type tasks_;
        std::mutex mutex_;
        std::condition_variable condition_;
        bool stop_ = false;
    };
}
