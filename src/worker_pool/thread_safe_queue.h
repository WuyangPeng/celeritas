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

        thread_safe_queue() noexcept = default;
        ~thread_safe_queue() noexcept = default;
        thread_safe_queue(const thread_safe_queue& rhs) = delete;
        thread_safe_queue& operator=(const thread_safe_queue& rhs) = delete;
        thread_safe_queue(thread_safe_queue&& rhs) noexcept;
        thread_safe_queue& operator=(thread_safe_queue&& rhs) noexcept;

        void push(std::function<void()> task);
        bool pop(std::function<void()>& task);
        void stop();

    private:
        std::queue<std::function<void()>> tasks_;
        std::mutex mutex_;
        std::condition_variable condition_;
        bool stop_ = false;
    };
}
