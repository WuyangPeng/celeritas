#include "task_progress.h"
#include "task_status_type.h"

celeritas::task_progress::task_progress(const int64_t id, const int cfg_id)
    : class_type{ id, cfg_id, 0 }
{
}

celeritas::task_progress::task_progress(const int64_t id, const int cfg_id, const int progress)
    : id_{ id }, cfg_id_{ cfg_id }, progress_{ progress }, status_{ task_status_type::in_progress }
{
}

int64_t celeritas::task_progress::get_id() const
{
    return id_;
}

int celeritas::task_progress::get_cfg_id() const
{
    return cfg_id_;
}

int celeritas::task_progress::get_progress() const
{
    return progress_;
}

celeritas::task_status_type celeritas::task_progress::get_status() const
{
    return status_;
}

void celeritas::task_progress::set_progress(const int progress)
{
    progress_ = progress;
}

void celeritas::task_progress::add_progress(const int progress)
{
    progress_ += progress;
}

void celeritas::task_progress::set_status(const task_status_type status)
{
    status_ = status;
}