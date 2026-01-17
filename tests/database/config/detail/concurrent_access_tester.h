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
        using any_io_executor = boost::asio::any_io_executor;

        explicit concurrent_access_tester(const any_io_executor& executor);

        void run();

        [[nodiscard]] void_awaitable_type wait();

    private:
        using atomic_int_shared_ptr = std::shared_ptr<std::atomic_int>;
        using exception_shared_ptr = std::shared_ptr<std::exception_ptr>;

        [[nodiscard]] static void_awaitable_type check_time_refresh_task();

        any_io_executor any_io_executor_;
        atomic_int_shared_ptr tasks_remaining_;
        exception_shared_ptr exception_;
    };
}


