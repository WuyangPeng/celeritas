#include "redis_context.h"
#include "../../common/core_utilities/celeritas_error.h"

using namespace std::literals;

celeritas::redis_context::redis_context(const std::string& host, const int port)
    : redis_context_{ redisConnect(host.c_str(), port) }
{
    if (redis_context_ == nullptr)
    {
        throw celeritas_error{ "failed to connect to redis server" };
    }

    if (redis_context_->err != REDIS_OK)
    {
        throw celeritas_error{ "failed to connect to redis server: "s + redis_context_->errstr };
    }
}

celeritas::redis_context::~redis_context() noexcept
{
    if (redis_context_ != nullptr)
    {
        redisFree(redis_context_);
    }
}

redisContext* celeritas::redis_context::get_redis_context() noexcept
{
    return redis_context_;
}