#include "redis_reply.h"
#include "common/celeritas_error.h"

using namespace std::literals;

celeritas::redis_reply::redis_reply(redis_context& redis_context, const std::string& command)
    : redis_reply_{ static_cast<redisReply*>(::redisCommand(redis_context.get_redis_context(), command.c_str())) }
{
    if (redis_reply_ == nullptr)
    {
        throw celeritas_error("command failed (NULL reply):  "s + redis_context.get_redis_context()->errstr);
    }

    if (redis_reply_->type == REDIS_REPLY_STATUS && redis_reply_->str == "OK"s)
    {
        return;
    }

    throw celeritas_error("command failed (NULL reply):  "s + redis_reply_->str);
}

celeritas::redis_reply::~redis_reply() noexcept
{
    ::freeReplyObject(redis_reply_);
}

redisReply* celeritas::redis_reply::GetRedisReply() noexcept
{
    return redis_reply_;
}