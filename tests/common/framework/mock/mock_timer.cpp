#include "mock_timer.h"

celeritas::mock_timer::mock_timer(const any_io_executor& any_io_executor, const duration_type interval, const bool disposable)
    : base_type{ any_io_executor, interval, disposable }
{
}

void celeritas::mock_timer::execute_timer_task()
{
    ++execution_count;
}

int celeritas::mock_timer::get_execution_count() const
{
    return execution_count;
}