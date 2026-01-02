#include "multiple_producers_fixture.h"

celeritas::multiple_producers_fixture::multiple_producers_fixture()
    : queue_{},
      counter_{ 0 },
      thread1_{ &class_type::produce_tasks, this },
      thread2_{ &class_type::produce_tasks, this }
{
}

void celeritas::multiple_producers_fixture::produce()
{
    thread1_.join();
    thread2_.join();

    consume_tasks();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    queue_.stop();
}

int celeritas::multiple_producers_fixture::get_counter()
{
    return counter_;
}

bool celeritas::multiple_producers_fixture::pop(task_type& task)
{
    return queue_.pop(task);
}

void celeritas::multiple_producers_fixture::produce_tasks()
{
    for (auto i = 0; i < tasks_per_producer; ++i)
    {
        queue_.push([this] {
            ++counter_;
        });
    }
}

void celeritas::multiple_producers_fixture::consume_tasks()
{
    auto executed_tasks = 0;
    thread_safe_queue::task_type task{};
    while (executed_tasks < total_tasks && queue_.pop(task))
    {
        task();
        ++executed_tasks;
    }
}