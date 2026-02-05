#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

#include <functional>

namespace celeritas
{
    class io_context_fixture
    {
    public:
        using class_type = io_context_fixture;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using awaitable_function = std::function<void_awaitable_type()>;

        io_context_fixture();

        virtual ~io_context_fixture() noexcept;

        void spawn(awaitable_function function);

        void run(awaitable_function function);

        void set_test_end(bool test_end);

    protected:
        boost::asio::io_context io_context_;
        bool test_end_;
    };
}
