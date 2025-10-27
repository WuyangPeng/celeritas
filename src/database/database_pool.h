#pragma once

#include "detail/cleanup_database_session_timer.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class database_pool : public std::enable_shared_from_this<database_pool>
    {
    public:
        using class_type = database_pool;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using error_code_type = boost::system::error_code;

        database_pool() noexcept = default;

        virtual ~database_pool() noexcept = default;

        database_pool(const database_pool& rhs) noexcept = delete;

        database_pool& operator=(const database_pool& rhs) noexcept = delete;

        database_pool(database_pool&& rhs) noexcept = delete;

        database_pool& operator=(database_pool&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type async_initialize() = 0;

        void start_cleanup_timer(io_context_type& io_context);

        void stop_cleanup_timer();

        virtual void cleanup_database_by_duration() = 0;

    private:
        using cleanup_database_session_timer_shared_ptr = std::shared_ptr<cleanup_database_session_timer>;

        cleanup_database_session_timer_shared_ptr cleanup_database_session_timer_;
    };
}
