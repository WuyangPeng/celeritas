#include "mock_application_loader.h"

void celeritas::mock_application_loader::submit_task(const task_type task)
{
    task_submitted_ = true;
    task();
}

bool celeritas::mock_application_loader::get_task_submitted() const
{
    return task_submitted_;
}