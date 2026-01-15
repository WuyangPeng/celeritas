#include "redis_reply.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/database_fwd.h"

#include <boost/numeric/conversion/cast.hpp>

celeritas::redis_reply::redis_reply(redis_context& redis_context, const std::string& command)
    : command_{ command },
      argv_{},
      argv_length_{},
      redis_reply_{ static_cast<redisReply*>(redisCommand(redis_context.get_redis_context(), command.c_str())) }
{
    LOG_CHANNEL(database_channel, debug) << "redis command: " << command;

    init(redis_context);
}

celeritas::redis_reply::redis_reply(redis_context& redis_context, const array_type& command)
    : command_{ command },
      argv_{ generate_argv(command) },
      argv_length_{ generate_argv_length(command) },
      redis_reply_{ static_cast<redisReply*>(redisCommandArgv(redis_context.get_redis_context(),
                                                              boost::numeric_cast<int>(command.size()),
                                                              argv_.data(),
                                                              argv_length_.data())) }
{
    init(redis_context);
}

celeritas::redis_reply::~redis_reply() noexcept
{
    freeReplyObject(redis_reply_);
}

int celeritas::redis_reply::to_integer() const
{
    if (redis_reply_->type == REDIS_REPLY_INTEGER)
    {
        return boost::numeric_cast<int>(redis_reply_->integer);
    }

    throw celeritas_error{ "redis reply type mismatch: expected integer" };
}

celeritas::redis_reply::optional_int celeritas::redis_reply::to_optional_int() const
{
    if (redis_reply_->type == REDIS_REPLY_NIL)
    {
        return std::nullopt;
    }

    if (redis_reply_->type == REDIS_REPLY_INTEGER)
    {
        return boost::numeric_cast<int>(redis_reply_->integer);
    }

    throw celeritas_error{ "redis reply type mismatch: expected integer" };
}

celeritas::redis_reply::optional_double celeritas::redis_reply::to_optional_double() const
{
    if (redis_reply_->type == REDIS_REPLY_NIL)
    {
        return std::nullopt;
    }

    if (redis_reply_->type == REDIS_REPLY_DOUBLE)
    {
        return redis_reply_->dval;
    }

    throw celeritas_error{ "Redis reply type mismatch: expected DOUBLE" };
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

    throw celeritas_error{ "reply type mismatch: Expected STRING or NIL, got type:{} ", redis_reply_->type };
}

celeritas::redis_reply::array_type celeritas::redis_reply::to_array() const
{
    if (redis_reply_->type == REDIS_REPLY_NIL)
    {
        return array_type{};
    }

    if (redis_reply_->type != REDIS_REPLY_ARRAY && redis_reply_->type != REDIS_REPLY_SET)
    {
        throw celeritas_error{ "reply type mismatch: Expected ARRAY, got type:{} ", redis_reply_->type };
    }

    return do_to_array();
}

celeritas::redis_reply::map_type celeritas::redis_reply::to_map() const
{
    if (redis_reply_->type != REDIS_REPLY_MAP)
    {
        throw celeritas_error{ "reply type mismatch: expected map for map conversion." };
    }

    const auto num_elements = redis_reply_->elements;

    if (num_elements % 2 != 0)
    {
        throw celeritas_error{ "map conversion failed: expected even number of elements for key-value map, got {}", num_elements };
    }

    if (num_elements == 0)
    {
        return map_type{};
    }

    return do_to_map();
}

celeritas::scan_result celeritas::redis_reply::to_scan_result() const
{
    if (redis_reply_->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error{ "reply type mismatch: expected array for scan result conversion." };
    }

    if (const auto num_elements = redis_reply_->elements;
        num_elements != 2)
    {
        throw celeritas_error{ " scan result num failed: got {}", num_elements };
    }

    auto cursor = get_cursor();

    auto keys = get_keys();

    return scan_result{ std::move(cursor), std::move(keys) };
}

celeritas::redis_reply::sorted_set_member_score_container celeritas::redis_reply::to_sorted_set_member_score() const
{
    if (redis_reply_->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error{ "reply type mismatch: expected array for sorted set member score result conversion." };
    }

    sorted_set_member_score_container result{};

    result.reserve(redis_reply_->elements);

    for (auto i = 0; i < redis_reply_->elements; ++i)
    {
        result.emplace_back(to_sorted_set_member_score(i));
    }

    return result;
}

void celeritas::redis_reply::init(redis_context& redis_context) const
{
    if (redis_reply_ == nullptr)
    {
        throw celeritas_error{ "command failed (NULL reply):  {}", redis_context.get_redis_context()->errstr };
    }

    std::string result_message{};
    if (redis_reply_->type == REDIS_REPLY_ERROR || redis_reply_->type == REDIS_REPLY_STATUS)
    {
        result_message = std::string{ redis_reply_->str, redis_reply_->len };
    }

    if (redis_reply_->type == REDIS_REPLY_ERROR)
    {
        throw celeritas_error{ "command failed (redis error reply):  {}", result_message };
    }

    // 特殊处理 AUTH 命令，确保它是 OK (如果需要严格检查)
    if (!command_.empty() && command_.at(0).find("AUTH") == 0 && redis_reply_->type == REDIS_REPLY_STATUS && result_message != redis_ok)
    {
        throw celeritas_error{ "command failed (not ok):  {}", result_message };
    }
}

std::string celeritas::redis_reply::to_string_from_element(const redisReply* element)
{
    switch (element->type)
    {
        case REDIS_REPLY_STRING:
        {
            return std::string{ element->str, element->len };
        }
        case REDIS_REPLY_NIL:
        {
            return ""; // 将 NIL 转换为 C++ 中的空字符串
        }
        case REDIS_REPLY_INTEGER:
        {
            return std::to_string(element->integer);
        }
        case REDIS_REPLY_DOUBLE:
        {
            return std::to_string(element->dval);
        }
        case REDIS_REPLY_ARRAY:
        {
            return array_to_string(element);
        }
        case REDIS_REPLY_MAP:
        {
            return map_to_string(element);
        }
        case REDIS_REPLY_ERROR:
        {
            // 如果子元素是错误，直接抛出异常
            throw celeritas_error{ "redis array element contained an error: " + std::string{ element->str, element->len } };
        }
        default:
        {
            // 处理其他不支持的类型
            throw celeritas_error{ "redis array element contained an unsupported type: {}", element->type };
        }
    }
}

std::string celeritas::redis_reply::get_cursor() const
{
    const auto cursor_element = redis_reply_->element[0];
    if (cursor_element->type != REDIS_REPLY_STRING)
    {
        throw celeritas_error{ "cursor Key element is not a string." };
    }
    return std::string{ cursor_element->str, cursor_element->len };
}

celeritas::redis_reply::array_type celeritas::redis_reply::get_keys() const
{
    const auto keys_element = redis_reply_->element[1];
    if (keys_element->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error{ "keys element is not a string." };
    }

    array_type keys{};
    for (auto i = 0; i < keys_element->elements; ++i)
    {
        std::string key{ keys_element->element[i]->str, keys_element->element[i]->len };
        keys.emplace_back(std::move(key));
    }
    return keys;
}

celeritas::redis_reply::command_array_type celeritas::redis_reply::generate_argv(const array_type& command)
{
    command_array_type result{};

    for (const auto& element : command)
    {
        result.emplace_back(element.c_str());
    }

    return result;
}

celeritas::redis_reply::command_length_array_type celeritas::redis_reply::generate_argv_length(const array_type& command)
{
    command_length_array_type result{};

    for (const auto& element : command)
    {
        result.emplace_back(element.size());
    }

    return result;
}

celeritas::redis_reply::array_type celeritas::redis_reply::do_to_array() const
{
    array_type result{};

    result.reserve(redis_reply_->elements);

    for (auto i = 0; i < redis_reply_->elements; ++i)
    {
        const auto element = redis_reply_->element[i];

        result.emplace_back(to_string_from_element(element));
    }

    return result;
}

celeritas::redis_reply::map_type celeritas::redis_reply::do_to_map() const
{
    map_type result{};

    for (auto i = 0; i < redis_reply_->elements; i += 2)
    {
        const auto key_element = redis_reply_->element[i];
        const auto value_element = redis_reply_->element[i + 1];

        if (key_element->type != REDIS_REPLY_STRING)
        {
            throw celeritas_error{ "map key element is not a string." };
        }

        std::string key{ key_element->str, key_element->len };

        auto value = to_string_from_element(value_element);

        result.emplace(std::move(key), std::move(value));
    }

    return result;
}

celeritas::sorted_set_member_score celeritas::redis_reply::to_sorted_set_member_score(const int index) const
{
    if (redis_reply_->element[index]->type != REDIS_REPLY_ARRAY)
    {
        throw celeritas_error{ "reply type mismatch: expected array for sorted set member score result conversion." };
    }

    if (redis_reply_->element[index]->elements != 2)
    {
        throw celeritas_error{ "reply type mismatch: expected array for sorted set member score result conversion." };
    }

    const auto key_element = redis_reply_->element[index]->element[0];
    const auto value_element = redis_reply_->element[index]->element[1];

    if (key_element->type != REDIS_REPLY_STRING)
    {
        throw celeritas_error{ "map key element is not a string." };
    }

    if (value_element->type != REDIS_REPLY_DOUBLE)
    {
        throw celeritas_error{ "map value element is not a double." };
    }

    std::string key{ key_element->str, key_element->len };

    const auto value = value_element->dval;

    return sorted_set_member_score{ std::move(key), value };
}

std::string celeritas::redis_reply::array_to_string(const redisReply* element)
{
    std::string result{};
    for (auto i = 0; i < element->elements; ++i)
    {
        result += to_string_from_element(element->element[i]);
        result += " ";
    }
    return result;
}

std::string celeritas::redis_reply::map_to_string(const redisReply* element)
{
    if (element->elements % 2 != 0)
    {
        throw celeritas_error{ "map conversion failed: expected even number of elements for key-value map, got {}", element->elements };
    }

    std::string result{};
    for (auto i = 0; i < element->elements; i += 2)
    {
        const auto key_element = element->element[i];
        const auto value_element = element->element[i + 1];

        if (key_element->type == REDIS_REPLY_STRING)
        {
            throw celeritas_error{ "Key element is not a string." };
        }

        result += to_string_from_element(key_element);
        result += ":";
        result += to_string_from_element(value_element);
        result += " ";
    }

    return result;
}
