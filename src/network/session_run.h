#pragma once

#include "common/common_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class session_run : public std::enable_shared_from_this<session_run>
    {
    public:
        using class_type = session_run;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_weak_ptr = std::weak_ptr<session>;

        session_run() noexcept = default;

        virtual ~session_run() noexcept = default;

        session_run(const session_run& rhs) = default;

        session_run& operator=(const session_run& rhs) = default;

        session_run(session_run&& rhs) noexcept = default;

        session_run& operator=(session_run&& rhs) noexcept = default;

        void start(const session_weak_ptr& session);

        [[nodiscard]] virtual void_awaitable_type run() = 0;

        [[nodiscard]] void_awaitable_type start_awaitable(const session_weak_ptr& session);

    protected:
        using session_shared_ptr = std::shared_ptr<session>;

        [[nodiscard]] session_shared_ptr get_session();

    private:
        virtual void do_start() = 0;

        session_weak_ptr session_;
    };
}