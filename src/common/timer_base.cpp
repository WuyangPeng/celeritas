#include "logger.h"
#include "noexcept_safe_call_and_log.h"
#include "timer_base.h"

celeritas::timer_base::timer_base(io_context_type& io_context, const duration_type interval)
    : timer_{ io_context }, interval_{ interval }
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

void celeritas::timer_base::set_duration_type(duration_type interval)
{
    interval_ = interval;
}

void celeritas::timer_base::on_timer_elapsed()
{
    try
    {
        execute_timer_task();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(common_channel, error) << "timer elapsed error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(common_channel, fatal) << "timer elapsed error: an unknown exception";
    }
}

void celeritas::timer_base::wait_for_next_tick()
{
    auto self = shared_from_this();

    timer_.expires_after(interval_);
    timer_.async_wait(
        [self](const error_code_type& error_code) {
            self->next_tick(error_code);
        });
}

void celeritas::timer_base::next_tick(const error_code_type& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        return;
    }

    this->on_timer_elapsed();

    wait_for_next_tick();
}