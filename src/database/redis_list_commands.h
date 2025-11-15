#pragma once

#include "database_fwd.h"
#include "redis_commands.h"

#include <boost/asio/awaitable.hpp>
#include <optional>
#include <string>
#include <vector>

namespace celeritas
{
    class redis_list_commands final : public redis_commands
    {
    public:
        using class_type = redis_list_commands;
        using base_type = redis_commands;

        using int_awaitable_type = boost::asio::awaitable<int>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using array_type = std::vector<std::string>;
        using array_awaitable_type = boost::asio::awaitable<array_type>;
        using blocking_left_pop_result_type = std::optional<std::pair<std::string, std::string> >;
        using blocking_left_pop_awaitable_type = boost::asio::awaitable<blocking_left_pop_result_type>;

        explicit redis_list_commands(redis_database_session& session) noexcept;

        // 将一个值推入列表左侧 (头部)。
        // 返回：推入操作后列表的长度。
        [[nodiscard]] int_awaitable_type async_left_push(const std::string& key, const std::string& value) const;

        // 将多个值推入列表左侧 (头部)。
        // 返回：推入操作后列表的长度。
        [[nodiscard]] int_awaitable_type async_left_push_many(const std::string& key, const key_container& values) const;

        // 将一个值推入列表右侧 (尾部)。
        // 返回：推入操作后列表的长度。
        [[nodiscard]] int_awaitable_type async_right_push(const std::string& key, const std::string& value) const;

        // 将多个值推入列表右侧 (尾部)。
        // 返回：推入操作后列表的长度。
        [[nodiscard]] int_awaitable_type async_right_push_many(const std::string& key, const key_container& values) const;

        // 移除并返回列表的第一个元素 (左侧/头部)。
        // 返回：列表的第一个元素，如果 key 不存在或列表为空，则返回 std::nullopt。
        [[nodiscard]] optional_string_awaitable_type async_left_pop(const std::string& key) const;

        // 移除并返回列表的最后一个元素 (右侧/尾部)。
        // 返回：列表的最后一个元素，如果 key 不存在或列表为空，则返回 std::nullopt。
        [[nodiscard]] optional_string_awaitable_type async_right_pop(const std::string& key) const;

        // 获取列表的长度。
        // 返回：列表的长度 (0 或正整数)。
        [[nodiscard]] int_awaitable_type async_get_length(const std::string& key) const;

        // 获取列表中指定范围内的元素。
        // start 和 end 都是闭区间索引 (0为第一个元素，-1为最后一个元素)。
        // 返回：范围内的元素列表 (std::vector<std::string>)。
        [[nodiscard]] array_awaitable_type async_get_range(const std::string& key, int start, int end) const;

        // 根据 count 的值，移除列表中与 value 相等的元素。
        // count > 0 : 从头到尾移除 count 个匹配项。
        // count < 0 : 从尾到头移除 count 个匹配项。
        // count = 0 : 移除所有匹配项。
        // 返回：被移除元素的数量。
        [[nodiscard]] int_awaitable_type async_remove(const std::string& key, int count, const std::string& value) const;

        // 移出并获取列表的第一个元素，如果列表为空，则阻塞。
        // keys: 要监听的列表 Key 集合 (至少一个)。
        // timeout_seconds: 阻塞的秒数。0 表示永远阻塞。
        // 返回：如果成功，返回 (key, value) 对，其中 key 是从中弹出元素的列表名；如果超时，返回 std::nullopt。
        [[nodiscard]] blocking_left_pop_awaitable_type async_blocking_left_pop(const key_container& keys, int timeout_seconds) const;
    };
}