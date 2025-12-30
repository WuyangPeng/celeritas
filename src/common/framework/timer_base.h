#pragma once

#include <boost/asio.hpp>

#include <chrono>
#include <memory>

namespace celeritas
{
    class timer_base : public std::enable_shared_from_this<timer_base>
    {
    public:
        using class_type = timer_base;

        using io_context_type = boost::asio::io_context;
        using duration_type = std::chrono::milliseconds;

        timer_base(io_context_type& io_context, duration_type interval, bool disposable = false);

        virtual ~timer_base() noexcept;

        timer_base(const timer_base& rhs) = delete;

        timer_base& operator=(const timer_base& rhs) = delete;

        timer_base(timer_base&& rhs) noexcept = delete;

        timer_base& operator=(timer_base&& rhs) noexcept = delete;

        void start(bool execute = false);

        void stop();

        void wait_for_next_tick();

        void set_duration_type(duration_type interval);

    private:
        using timer_type = boost::asio::steady_timer;
        using error_code_type = boost::system::error_code;

        void on_timer_elapsed();

        void next_tick(const error_code_type& error_code);

        virtual void execute_timer_task() = 0;

        timer_type timer_;
        duration_type interval_;
        bool disposable_;
    };
}