#include "multiple_consumers_fixture.h"

void celeritas::multiple_consumers_fixture::prepare_tasks()
{
    for (auto i = 0; i < num_tasks; ++i)
    {
        queue_.push([this] {
            ++counter_;
        });
    }
}

void celeritas::multiple_consumers_fixture::run_consumers()
{
    for (auto i = 0; i < num_consumers; ++i)
    {
        consumers_.emplace_back(&class_type::consumer_thread_func, this);
    }

    while (consumed_count_ < num_tasks)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    queue_.stop();

    for (auto& t : consumers_)
    {
        t.join();
    }
}

int celeritas::multiple_consumers_fixture::get_counter() const
{
    return counter_;
}

int celeritas::multiple_consumers_fixture::get_consumed_count() const
{
    return consumed_count_;
}

void celeritas::multiple_consumers_fixture::consumer_thread_func()
{
    thread_safe_queue::task_type task{};
    while (queue_.pop(task))
    {
        task();
        ++consumed_count_;
    }
}