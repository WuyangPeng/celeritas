#pragma once

#include <boost/asio/awaitable.hpp>

#include <atomic>
#include <memory>

namespace celeritas
{
    class concurrent_access_tester
    {
    public:
        using class_type = concurrent_access_tester;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;

        explicit concurrent_access_tester(io_context_type& io_context);

        void run();

        [[nodiscard]] void_awaitable_type wait();

    private:
        using atomic_int_shared_ptr = std::shared_ptr<std::atomic_int>;
        using exception_shared_ptr = std::shared_ptr<std::exception_ptr>;

        [[nodiscard]] static void_awaitable_type check_time_refresh_task();

        io_context_type& io_context_;
        atomic_int_shared_ptr tasks_remaining_;
        exception_shared_ptr exception_;
    };
}


