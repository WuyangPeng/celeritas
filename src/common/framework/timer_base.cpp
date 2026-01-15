#include "timer_base.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"

celeritas::timer_base::timer_base(const any_io_executor& any_io_executor, const duration_type interval, const bool disposable)
    : timer_{ any_io_executor }, interval_{ interval }, disposable_{ disposable }
{
}

celeritas::timer_base::~timer_base() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   this->stop();
                               },
                               common_channel,
                               "timer stop error: ");
}

void celeritas::timer_base::start(const bool execute)
{
    if (execute)
    {
        on_timer_elapsed();
    }

    wait_for_next_tick();
}

void celeritas::timer_base::stop()
{
    timer_.cancel();
}

void celeritas::timer_base::wait_for_next_tick()
{
    timer_.expires_after(interval_);
    timer_.async_wait(
        [self = shared_from_this()](const error_code_type& error_code) {
            self->next_tick(error_code);
        });
}

void celeritas::timer_base::set_duration_type(duration_type interval)
{
    interval_ = interval;
}

void celeritas::timer_base::on_timer_elapsed()
{
    noexcept_safe_call_and_log([self = shared_from_this()] {
                                   self->execute_timer_task();
                               },
                               common_channel,
                               "timer elapsed error: ");
}

void celeritas::timer_base::next_tick(const error_code_type& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        return;
    }

    this->on_timer_elapsed();
    if (!disposable_)
    {
        wait_for_next_tick();
    }
}