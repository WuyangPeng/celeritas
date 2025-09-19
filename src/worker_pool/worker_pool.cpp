#include "worker_pool.h"

celeritas::worker_pool::worker_pool(size_t num_threads)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back([this] {
            while (true)
            {
                std::function<void()> task;
                if (!queue_.pop(task))
                {
                    break;
                }
                task();
            }
        });
    }
}

celeritas::worker_pool::~worker_pool() noexcept
{
    queue_.stop();
}

celeritas::worker_pool::worker_pool(worker_pool&& rhs) noexcept
    : queue_(std::move(rhs.queue_)), workers_(std::move(rhs.workers_))
{
}

celeritas::worker_pool& celeritas::worker_pool::operator=(worker_pool&& rhs) noexcept
{
    if (this != &rhs)
    {
        queue_ = std::move(rhs.queue_);
        workers_ = std::move(rhs.workers_);
    }

    return *this;
}

void celeritas::worker_pool::submit(std::function<void()> task)
{
    queue_.push(std::move(task));
}