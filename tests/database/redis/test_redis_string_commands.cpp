#include "database/redis/redis_string_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_FIXTURE_TEST_SUITE(redis_string_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_set)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key" };
            const std::string value{ "test_value" };

            // 测试设置键值对
            const auto result = co_await string_commands.async_set(key, value);
            BOOST_CHECK_EQUAL(result, true);

            // 测试获取设置的值
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_with_expire)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_expire" };
            const std::string value{ "test_value_expire" };
            constexpr auto expire_seconds = 1;

            // 测试设置键值对并设置过期时间
            const auto result = co_await string_commands.async_set(key, value, expire_seconds);
            BOOST_CHECK_EQUAL(result, true);

            // 测试获取设置的值
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_not_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_nx" };
            const std::string value1{ "test_value1" };
            const std::string value2{ "test_value2" };
            constexpr auto expire_seconds = 10;

            // 测试第一次设置（键不存在）
            const auto result1 = co_await string_commands.async_set_not_exists(key, value1, expire_seconds);
            BOOST_CHECK_EQUAL(result1, true);

            // 测试第二次设置（键已存在）
            const auto result2 = co_await string_commands.async_set_not_exists(key, value2, expire_seconds);
            BOOST_CHECK_EQUAL(result2, false);

            // 验证值没有被修改
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value1);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_xx" };
            const std::string value1{ "test_value1" };
            const std::string value2{ "test_value2" };

            co_await session->get_redis_key_commands().async_delete(key);

            // 测试设置不存在的键
            const auto result1 = co_await string_commands.async_set_exists(key, value1);
            BOOST_CHECK_EQUAL(result1, false);

            // 先设置键
            co_await string_commands.async_set(key, value1);

            // 测试设置已存在的键
            const auto result2 = co_await string_commands.async_set_exists(key, value2);
            BOOST_CHECK_EQUAL(result2, true);

            // 验证值被修改
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            celeritas::redis_string_commands::key_value_container key_values{ { "test_key1", "test_value1" },
                                                                              { "test_key2", "test_value2" },
                                                                              { "test_key3", "test_value3" } };

            // 测试批量设置
            const auto result = co_await string_commands.async_set_many(key_values);
            BOOST_CHECK_EQUAL(result, true);

            // 验证设置的值
            for (const auto& [key, value] : key_values)
            {
                const auto get_result = co_await string_commands.async_get(key);
                BOOST_CHECK(get_result.has_value());
                BOOST_CHECK_EQUAL(get_result.value(), value);
            }

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_append)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_append" };
            const std::string value1{ "test_value" };
            const std::string value2{ "_append" };

            // 先设置初始值
            co_await string_commands.async_set(key, value1);

            // 测试追加值
            const auto result = co_await string_commands.async_append(key, value2);
            BOOST_CHECK_EQUAL(result, value1.length() + value2.length());

            // 验证结果
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value1 + value2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_incr_by" };
            co_await session->get_redis_key_commands().async_delete(key);

            constexpr auto increment = 5;

            // 测试增加指定值
            const auto result = co_await string_commands.async_increment_by(key, increment);
            BOOST_CHECK_EQUAL(result, increment);

            // 测试再次增加
            const auto result2 = co_await string_commands.async_increment_by(key, increment);
            BOOST_CHECK_EQUAL(result2, increment * 2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_incr" };
            co_await session->get_redis_key_commands().async_delete(key);
            // 测试增加1
            const auto result = co_await string_commands.async_increment(key);
            BOOST_CHECK_EQUAL(result, 1);

            // 测试再次增加1
            const auto result2 = co_await string_commands.async_increment(key);
            BOOST_CHECK_EQUAL(result2, 2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_decrement_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_decr_by" };
            constexpr auto decrement = 3;

            // 先设置一个初始值
            co_await string_commands.async_set(key, "10");

            // 测试减少指定值
            const auto result = co_await string_commands.async_decrement_by(key, decrement);
            BOOST_CHECK_EQUAL(result, 10 - decrement);

            // 测试再次减少
            const auto result2 = co_await string_commands.async_decrement_by(key, decrement);
            BOOST_CHECK_EQUAL(result2, 10 - decrement * 2);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_decrement)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_decr" };

            // 先设置一个初始值
            co_await string_commands.async_set(key, "5");

            // 测试减少1
            const auto result = co_await string_commands.async_decrement(key);
            BOOST_CHECK_EQUAL(result, 4);

            // 测试再次减少1
            const auto result2 = co_await string_commands.async_decrement(key);
            BOOST_CHECK_EQUAL(result2, 3);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_get" };
            const std::string value{ "test_value_get" };
            co_await session->get_redis_key_commands().async_delete(key);

            // 测试获取不存在的键
            const auto result1 = co_await string_commands.async_get(key);
            BOOST_CHECK(!result1.has_value());

            // 设置键值对
            co_await string_commands.async_set(key, value);

            // 测试获取存在的键
            const auto result2 = co_await string_commands.async_get(key);
            BOOST_CHECK(result2.has_value());
            BOOST_CHECK_EQUAL(result2.value(), value);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            // 设置一些键值对
            co_await string_commands.async_set("test_key_many1", "test_value_many1");
            co_await string_commands.async_set("test_key_many2", "test_value_many2");

            // 测试批量获取
            const celeritas::redis_string_commands::key_container keys{ "test_key_many1", "test_key_many2", "test_key_many3" };
            const auto result = co_await string_commands.async_get_many(keys);

            BOOST_CHECK_EQUAL(result.size(), keys.size());
            BOOST_CHECK_EQUAL(result[0], "test_value_many1");
            BOOST_CHECK_EQUAL(result[1], "test_value_many2");
            BOOST_CHECK_EQUAL(result[2], ""); // 不存在的键返回空字符串

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_set)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_get_set" };
            const std::string value1{ "test_value1" };
            const std::string value2{ "test_value2" };

            co_await session->get_redis_key_commands().async_delete(key);

            // 测试获取并设置不存在的键
            const auto result1 = co_await string_commands.async_get_set(key, value1);
            BOOST_CHECK(!result1.has_value());

            // 测试获取并设置已存在的键
            const auto result2 = co_await string_commands.async_get_set(key, value2);
            BOOST_CHECK(result2.has_value());
            BOOST_CHECK_EQUAL(result2.value(), value1);

            // 验证新值已设置
            const auto result3 = co_await string_commands.async_get(key);
            BOOST_CHECK(result3.has_value());
            BOOST_CHECK_EQUAL(result3.value(), value2);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
