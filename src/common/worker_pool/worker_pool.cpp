#include "worker_pool.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"

celeritas::worker_pool::worker_pool(const int num_threads)
    : queue_{}, workers_{}
{
    init_work(num_threads);
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

void celeritas::worker_pool::init_work(const int num_threads)
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

bool celeritas::worker_pool::execute_task() noexcept
{
    return noexcept_safe_call_and_log([this] {
                                          return this->get_and_run_task();
                                      },
                                      common_channel,
                                      "Task threw an exception: ",
                                      false);
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
