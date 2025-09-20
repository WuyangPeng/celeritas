#include "thread_safe_queue.h"

void celeritas::thread_safe_queue::push(task_type task)
{
    std::unique_lock lock{ mutex_ };

    tasks_.push(std::move(task));
    condition_.notify_one();
}

bool celeritas::thread_safe_queue::pop(task_type& task)
{
    std::unique_lock lock{ mutex_ };

    condition_.wait(lock, [this] {
        return !tasks_.empty() || stop_;
    });

    if (stop_ && tasks_.empty())
    {
        return false;
    }

    task = std::move(tasks_.front());
    tasks_.pop();

    return true;
}

void celeritas::thread_safe_queue::stop()
{
    std::unique_lock lock{ mutex_ };
    stop_ = true;
    condition_.notify_all();
}