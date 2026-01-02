#include "multiple_tasks_fixture.h"

celeritas::multiple_tasks_fixture::multiple_tasks_fixture()
    : pool_{ 4 }, counter_{ 0 }, futures_{}
{
}

void celeritas::multiple_tasks_fixture::submit_tasks()
{
    for (auto i = 0; i < num_tasks; ++i)
    {
        do_submit_tasks();
    }
}

void celeritas::multiple_tasks_fixture::wait_for_tasks()
{
    for (auto& future : futures_)
    {
        future.get();
    }
}

int celeritas::multiple_tasks_fixture::get_counter()
{
    return counter_;
}

void celeritas::multiple_tasks_fixture::do_submit_tasks()
{
    auto task = std::make_shared<std::packaged_task<void()> >([this] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ++counter_;
    });

    futures_.emplace_back(task->get_future());
    pool_.submit([task] {
        (*task)();
    });
}