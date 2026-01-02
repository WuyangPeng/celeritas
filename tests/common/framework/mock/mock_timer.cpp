#include "mock_timer.h"

celeritas::mock_timer::mock_timer(io_context_type& io_context, const duration_type interval, const bool disposable)
    : base_type{ io_context, interval, disposable }
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