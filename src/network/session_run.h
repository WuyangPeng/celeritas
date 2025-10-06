#pragma once

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class session_run : public std::enable_shared_from_this<session_run>
    {
    public:
        using class_type = session_run;
        using void_awaitable_type = boost::asio::awaitable<void>;

        session_run() noexcept = default;

        virtual ~session_run() noexcept = default;

        session_run(const session_run& rhs) = default;

        session_run& operator=(const session_run& rhs) = default;

        session_run(session_run&& rhs) noexcept = default;

        session_run& operator=(session_run&& rhs) noexcept = default;

        virtual void start() = 0;

        [[nodiscard]] virtual void_awaitable_type run() = 0;
    };
}