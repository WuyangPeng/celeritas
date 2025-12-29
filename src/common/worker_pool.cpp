#include "worker_pool.h"
#include "common/logger.h"
#include "core_utilities/noexcept_safe_call_and_log.h"

celeritas::worker_pool::worker_pool(const int num_threads)
{
    for (auto i = 0; i < num_threads; ++i)
    {
        add_work();
    }
}

celeritas::worker_pool::~worker_pool() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   this->queue_.stop();
                               },
                               common_channel,
                               "error while stopping thread_safe_queue: ");
}

void celeritas::worker_pool::submit(task_type task)
{
    queue_.push(std::move(task));
}

void celeritas::worker_pool::add_work()
{
    workers_.emplace_back([this] {
        while (execute_task())
        {
        }
    });
}

bool celeritas::worker_pool::execute_task()
{
    try
    {
        return get_and_run_task();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(common_channel, error) << "Task threw an exception: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(common_channel, fatal) << "Task threw an unknown exception";
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

    LOG_CHANNEL(common_channel, debug) << "task is run.";

    return true;
}
