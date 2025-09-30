#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "detail/redis_reply.h"

using namespace std::literals;

celeritas::redis_database_session::redis_database_session(const std::string_view& host,
                                                          uint16_t port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          boost::asio::io_context& io_context)
    : redis_context_{}, io_context_{ io_context }, host_{ host }, password_{ password }, user_{ user }, port_{ port }, db_name_{ db_name }
{
}

celeritas::redis_database_session::awaitable_type celeritas::redis_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    redis_context_ = std::make_unique<redis_context>(host_, port_);

    auto command = user_.empty() ? "AUTH " + password_ : "AUTH " + user_ + " " + password_;

    redis_reply redis_reply{ *redis_context_.get(), command };

    LOG_CHANNEL(database_channel, info) << "Authentication successful (AUTH: OK).";
}

