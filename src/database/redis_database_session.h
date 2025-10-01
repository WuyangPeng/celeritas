#pragma once

#include "database_session.h"
#include "detail/redis_context.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <memory>
#include <string>

namespace celeritas
{
    class redis_database_session final : public database_session
    {
    public:
        using class_type = redis_database_session;
        using base_type = database_session;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;

        explicit redis_database_session(const std::string_view& host,
                                        int port,
                                        const std::string_view& user,
                                        const std::string_view& password,
                                        const std::string_view& url,
                                        const std::string_view& db_name,
                                        io_context_type& io_context);

        ~redis_database_session() noexcept override = default;

        redis_database_session(const redis_database_session& rhs) noexcept = delete;

        redis_database_session& operator=(const redis_database_session& rhs) noexcept = delete;

        redis_database_session(redis_database_session&& rhs) noexcept = delete;

        redis_database_session operator=(redis_database_session&& rhs) noexcept = delete;

        // 异步连接到Redis
        [[nodiscard]] void_awaitable_type async_connect();

    private:
        using redis_context_unique_ptr = std::unique_ptr<redis_context>;

        redis_context_unique_ptr redis_context_;
        io_context_type& io_context_;

        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
    };
}
