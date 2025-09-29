#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"

using namespace std::literals;

celeritas::redis_database_session::redis_database_session(const std::string_view& host, uint16_t port, const std::string_view& password, boost::asio::io_context& io_context)
    : connection_{}, io_context_{ io_context }, host_{ host }, password_{ password }, port_{ port }
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

    if (connection_ == nullptr)
    {
        throw celeritas_error("failed to connect to redis server");
    }

    if (connection_->err)
    {
        // 连接到 Redis 服务器失败的真实原因
        std::string error_msg = connection_->errstr;
        close_connection();
        throw celeritas_error("failed to connect to redis server: " + error_msg);
    }

    // 假设使用 Redis 默认的 AUTH <password> 格式
    redisReply* reply = static_cast<redisReply*>(redisCommand(connection_, "AUTH %s", password_.c_str()));

    // 检查 AUTH 命令的回复
    if (reply == nullptr)
    {
        close_connection();
        throw celeritas_error("AUTH command failed (NULL reply):  "s + connection_->errstr);
    }

    // AUTH 成功会返回状态回复 (REDIS_REPLY_STATUS) 且内容为 "OK"
    if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0)
    {
        LOG_CHANNEL(database_channel, error) << "Authentication successful (AUTH: OK).";

        freeReplyObject(reply);

        co_return;
    }

    throw celeritas_error("AUTH command failed (NULL reply):  "s + connection_->errstr);
}

void celeritas::redis_database_session::close_connection()
{
    if (connection_ != nullptr)
    {
        ::redisFree(connection_);
    }

    connection_ = nullptr;
}