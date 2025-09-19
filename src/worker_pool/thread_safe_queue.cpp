#include "thread_safe_queue.h"

celeritas::thread_safe_queue::thread_safe_queue(thread_safe_queue&& rhs) noexcept
    : tasks_{ std::move(rhs.tasks_) }, mutex_{}, condition_{}, stop_{ rhs.stop_ }
{
}

celeritas::thread_safe_queue& celeritas::thread_safe_queue::operator=(thread_safe_queue&& rhs) noexcept
{
    tasks_ = std::move(rhs.tasks_);
    stop_ = rhs.stop_;

    return *this;
}

void celeritas::thread_safe_queue::push(std::function<void()> task)
{
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push(std::move(task));
    condition_.notify_one();
}

bool celeritas::thread_safe_queue::pop(std::function<void()>& task)
{
    std::unique_lock<std::mutex> lock(mutex_);
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
    std::unique_lock<std::mutex> lock(mutex_);
    stop_ = true;
    condition_.notify_all();
}