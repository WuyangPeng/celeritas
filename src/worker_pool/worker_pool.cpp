#include "common/logger.h"
#include "worker_pool.h"

celeritas::worker_pool::worker_pool(const int num_threads)
{
    for (auto i = 0; i < num_threads; ++i)
    {
        add_work();
    }
}

void celeritas::worker_pool::add_work()
{
    workers_.emplace_back([this] {
        while (execute_task())
        {
        }
    });
}

celeritas::worker_pool::~worker_pool() noexcept
{
    queue_.stop();
}

void celeritas::worker_pool::submit(task_type task)
{
    queue_.push(std::move(task));
}

bool celeritas::worker_pool::execute_task()
{
    try
    {
        return get_and_run_task();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(worker_pool_channel, error) << "Task threw an exception: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(worker_pool_channel, fatal) << "Task threw an unknown exception";
    }

    return true;
}

bool celeritas::worker_pool::get_and_run_task()
{
    task_type task{};
    if (!queue_.pop(task))
    {
        return false;
    }
    task();

    return true;
}
