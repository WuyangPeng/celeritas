#pragma once

#include "database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <string>
#include <hiredis.h>
#include <memory>

namespace celeritas
{
    class redis_database_session : public database_session
    {
    public:
        using class_type = redis_database_session;

        using awaitable_type = boost::asio::awaitable<void>;
        using command_awaitable_type = boost::asio::awaitable<std::shared_ptr<::redisReply> >;

        explicit redis_database_session(const std::string_view& host,
                                        uint16_t port,
                                        const std::string_view& user,
                                        const std::string_view& password,
                                        const std::string_view& url,
                                        const std::string_view& db_name,
                                        boost::asio::io_context& io_context);

        ~redis_database_session() noexcept override;

        redis_database_session(const redis_database_session& rhs) noexcept = delete;

        redis_database_session& operator=(const redis_database_session& rhs) noexcept = delete;

        redis_database_session(redis_database_session&& rhs) noexcept = delete;

        redis_database_session operator=(redis_database_session&& rhs) noexcept = delete;

        // 异步连接到Redis
        [[nodiscard]] awaitable_type async_connect();

    private:
        ::redisContext* connection_;
        boost::asio::io_context& io_context_;

        void close_connection();

        std::string host_;
        uint16_t port_ = 0;
        std::string password_;
    };
}
