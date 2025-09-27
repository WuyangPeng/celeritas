#include "redis_database_session.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::redis_database_session::redis_database_session(const std::string_view& host, uint16_t port, boost::asio::io_context& io_context)
    : connection_{}, io_context_{ io_context }, host_{ host }, port_{ port }
{
}

celeritas::redis_database_session::~redis_database_session() noexcept
{
    close_connection();
}

celeritas::redis_database_session::awaitable_type celeritas::redis_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    connection_ = ::redisConnect(host_.c_str(), port_);

    co_return;
}

void celeritas::redis_database_session::close_connection()
{
    ::redisFree(connection_);

    connection_ = nullptr;
}