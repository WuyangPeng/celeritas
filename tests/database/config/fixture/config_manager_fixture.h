#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace celeritas
{
    class config_manager_fixture
    {
    public:
        using class_type = config_manager_fixture;
        using io_context_type = boost::asio::io_context;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;

        config_manager_fixture();

        ~config_manager_fixture();

        void run(awaitable_function func);

        [[nodiscard]] io_context_type& get_io_context();

        void set_test_end(bool test_end);

    private:
        io_context_type io_context_;
        bool test_end_;
    };
}