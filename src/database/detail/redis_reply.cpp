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

    std::string result_message{};
    if (redis_reply_->type == REDIS_REPLY_ERROR || redis_reply_->type == REDIS_REPLY_STATUS)
    {
        result_message = std::string{ redis_reply_->str, redis_reply_->len };
    }

    if (redis_reply_->type == REDIS_REPLY_ERROR)
    {
        throw celeritas_error("command failed (Redis ERROR reply):  " + result_message);
    }

    // 特殊处理 AUTH 命令，确保它是 OK (如果需要严格检查)
    if (command.find("AUTH") == 0 && redis_reply_->type == REDIS_REPLY_STATUS && result_message != "OK")
    {
        throw celeritas_error("command failed (AUTH NOT OK):  " + result_message);
    }
}

celeritas::redis_reply::~redis_reply() noexcept
{
    ::freeReplyObject(redis_reply_);
}

redisReply* celeritas::redis_reply::GetRedisReply() noexcept
{
    return redis_reply_;
}

int celeritas::redis_reply::to_integer() const
{
    if (redis_reply_->type == REDIS_REPLY_INTEGER)
    {
        return static_cast<int>(redis_reply_->integer);
    }

    throw celeritas_error("Redis reply type mismatch: expected INTEGER");
}

celeritas::redis_reply::optional_string celeritas::redis_reply::to_optional_string() const
{
    if (redis_reply_->type == REDIS_REPLY_NIL)
    {
        return std::nullopt;
    }

    if (redis_reply_->type == REDIS_REPLY_STRING || redis_reply_->type == REDIS_REPLY_STATUS)
    {
        return std::string{ redis_reply_->str, redis_reply_->len };
    }

    throw celeritas_error("Reply type mismatch: Expected STRING or NIL, got type " + std::to_string(redis_reply_->type));
}

celeritas::redis_reply::array_type celeritas::redis_reply::to_array() const
{
    if (redis_reply_->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error("Reply type mismatch: Expected ARRAY, got type " + std::to_string(redis_reply_->type));
    }

    array_type result{};

    result.reserve(redis_reply_->elements);

    for (auto i = 0; i < redis_reply_->elements; ++i)
    {
        switch (const auto element = redis_reply_->element[i];
            element->type)
        {
            case REDIS_REPLY_STRING:
            {
                result.emplace_back(element->str, element->len);
                break;
            }
            case REDIS_REPLY_NIL:
            {
                result.emplace_back("");
                break;
            }
            case REDIS_REPLY_INTEGER:
            {
                result.emplace_back(std::to_string(element->integer));
                break;
            }
            case REDIS_REPLY_ERROR:
            {
                throw celeritas_error("Redis array element contained an ERROR: " + std::string{ element->str, element->len });
            }
            default:
            {
                throw celeritas_error("Redis array element contained an unsupported type: " + std::to_string(element->type));
            }
        }
    }

    return result;
}

celeritas::redis_reply::map_type celeritas::redis_reply::to_map() const
{
    if (redis_reply_->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error("Reply type mismatch: Expected ARRAY for map conversion.");
    }

    const auto num_elements = redis_reply_->elements;

    if (num_elements % 2 != 0)
    {
        throw celeritas_error("Map conversion failed: Expected even number of elements for Key-Value map, got " + std::to_string(num_elements));
    }

    if (num_elements == 0)
    {
        return {};
    }

    map_type result{};

    for (auto i = 0; i < num_elements; i += 2)
    {
        const auto key_element = redis_reply_->element[i];
        const auto value_element = redis_reply_->element[i + 1];

        if (key_element->type != REDIS_REPLY_STRING)
        {
            throw celeritas_error("Map Key element is not a STRING.");
        }
        std::string key{ key_element->str, key_element->len };

        std::string value{};
        if (value_element->type == REDIS_REPLY_STRING)
        {
            value.assign(value_element->str, value_element->len);
        }
        else if (value_element->type == REDIS_REPLY_NIL)
        {
            value = "";
        }
        else if (value_element->type == REDIS_REPLY_INTEGER)
        {
            value = std::to_string(value_element->integer);
        }
        else
        {
            throw celeritas_error("Map Value element is of unsupported type.");
        }

        result.emplace(std::move(key), std::move(value));
    }

    return result;
}
