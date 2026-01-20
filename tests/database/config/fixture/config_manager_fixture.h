#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

#include <atomic>
#include <memory>

namespace celeritas
{
    class config_manager_fixture
    {
    public:
        using class_type = config_manager_fixture;
        using io_context_type = boost::asio::io_context;
        using void_awaitable = boost::asio::awaitable<void>;
        using awaitable_function = std::function<void_awaitable()>;
        using atomic_int_shared_ptr = std::shared_ptr<std::atomic_int>;
        using atomic_bool_shared_ptr = std::shared_ptr<std::atomic_bool>;

        config_manager_fixture();

        ~config_manager_fixture();

        void spawn(awaitable_function func);

        void run(awaitable_function func);

        [[nodiscard]] io_context_type& get_io_context();

        void set_test_end(bool test_end);

        static void check_time_refresh_valid();

        void spawn_writer(const atomic_int_shared_ptr& tasks_remaining, const atomic_bool_shared_ptr& stop_flag);

        void spawn_reader(const atomic_int_shared_ptr& tasks_remaining, const atomic_bool_shared_ptr& stop_flag);

    private:
        io_context_type io_context_;
        bool test_end_;
    };
}
