#include "common/logger.h"
#include "worker_pool.h"

celeritas::worker_pool::worker_pool(size_t num_threads)
{
    for (auto i = 0u; i < num_threads; ++i)
    {
        workers_.emplace_back([this] {
            for (;;)
            {
                task_type task{};
                if (!queue_.pop(task))
                {
                    break;
                }
                execute_task(task);
            }
        });
    }
}

celeritas::worker_pool::~worker_pool() noexcept
{
    queue_.stop();
}

void celeritas::worker_pool::submit(task_type task)
{
    queue_.push(std::move(task));
}

void celeritas::worker_pool::execute_task(const task_type& task)
{
    try
    {
        task();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(worker_pool_channel, error) << "Task threw an exception: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(worker_pool_channel, fatal) << "Task threw an unknown exception";
    }
}