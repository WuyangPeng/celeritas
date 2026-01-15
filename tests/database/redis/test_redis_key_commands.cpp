#include "database/redis/redis_key_commands.h"
#include "database/redis/redis_string_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <set>

BOOST_FIXTURE_TEST_SUITE(redis_key_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_delete)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_delete" };
            const std::string value{ "test_value" };

            // 设置一个键值对
            co_await string_commands.async_set(key, value);

            // 验证键存在
            const auto exists_before = co_await key_commands.async_is_exists(key);
            BOOST_CHECK_EQUAL(exists_before, true);

            // 删除键
            const auto result = co_await key_commands.async_delete(key);
            BOOST_CHECK_EQUAL(result, 1); // 应该删除了1个键

            // 验证键已被删除
            const auto exists_after = co_await key_commands.async_is_exists(key);
            BOOST_CHECK_EQUAL(exists_after, false);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_nonexistent_key)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();

            const std::string key{ "test_key_delete_nonexistent" };

            // 删除不存在的键
            const auto result = co_await key_commands.async_delete(key);
            BOOST_CHECK_EQUAL(result, 0); // 没有删除任何键

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key1{ "test_key_delete_many1" };
            const std::string key2{ "test_key_delete_many2" };
            const std::string key3{ "test_key_delete_many3" };
            const std::string value{ "test_value" };

            // 设置多个键值对
            co_await string_commands.async_set(key1, value);
            co_await string_commands.async_set(key2, value);
            co_await string_commands.async_set(key3, value);

            // 验证键存在
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key1), true);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key2), true);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key3), true);

            // 删除多个键
            const celeritas::redis_commands::key_container keys{ key1, key2, key3 };
            const auto result = co_await key_commands.async_delete_many(keys);
            BOOST_CHECK_EQUAL(result, 3); // 应该删除了3个键

            // 验证键已被删除
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key1), false);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key2), false);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key3), false);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_many_partial_existence)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key1{ "test_key_delete_partial1" };
            const std::string key2{ "test_key_delete_partial2" };
            const std::string key3{ "test_key_delete_partial3" };
            const std::string value{ "test_value" };

            // 只设置部分键
            co_await string_commands.async_set(key1, value);
            co_await string_commands.async_set(key3, value);

            // 删除包含存在和不存在的键
            const celeritas::redis_commands::key_container keys{ key1, key2, key3 }; // key2不存在
            const auto result = co_await key_commands.async_delete_many(keys);
            BOOST_CHECK_EQUAL(result, 2); // 应该删除了2个键（key1和key3）

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_expire_seconds)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_expire" };
            const std::string value{ "test_value" };
            constexpr int expire_seconds = 10;

            // 设置一个键值对
            co_await string_commands.async_set(key, value);

            // 设置过期时间
            const auto result = co_await key_commands.async_set_expire_seconds(key, expire_seconds);
            BOOST_CHECK_EQUAL(result, true); // 应该成功设置过期时间

            // 检查剩余过期时间
            const auto ttl = co_await key_commands.async_get_expire_seconds(key);
            BOOST_CHECK(ttl > 0 && ttl <= expire_seconds); // 剩余时间应在0到设置值之间

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_expire_seconds)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key1{ "test_key_ttl1" };
            const std::string key2{ "test_key_ttl2" };
            const std::string value{ "test_value" };
            constexpr int expire_seconds = 10;

            // 设置一个键值对
            co_await string_commands.async_set(key1, value, -1);

            // 设置带过期时间的键
            co_await string_commands.async_set(key2, value, expire_seconds);

            // 检查永不过期的键
            const auto ttl1 = co_await key_commands.async_get_expire_seconds(key1);
            BOOST_CHECK_EQUAL(ttl1, -1); // 永不过期

            // 检查带过期时间的键
            const auto ttl2 = co_await key_commands.async_get_expire_seconds(key2);
            BOOST_CHECK(ttl2 > 0 && ttl2 <= expire_seconds); // 应该有一个正的剩余时间

            // 检查不存在的键
            const auto ttl3 = co_await key_commands.async_get_expire_seconds("nonexistent_key");
            BOOST_CHECK_EQUAL(ttl3, -2); // 键不存在

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_is_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_exists" };
            const std::string value{ "test_value" };
            co_await session->get_redis_key_commands().async_delete(key);
            // 检查不存在的键
            const auto exists1 = co_await key_commands.async_is_exists(key);
            BOOST_CHECK_EQUAL(exists1, false);

            // 设置一个键值对
            co_await string_commands.async_set(key, value);

            // 检查存在的键
            const auto exists2 = co_await key_commands.async_is_exists(key);
            BOOST_CHECK_EQUAL(exists2, true);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_is_exists_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key1{ "test_key_exists_many1" };
            const std::string key2{ "test_key_exists_many2" };
            const std::string key3{ "test_key_exists_many3" };
            const std::string value{ "test_value" };
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2, key3 });
            // 设置部分键
            co_await string_commands.async_set(key1, value);
            co_await string_commands.async_set(key3, value);

            // 检查多个键的存在性
            const celeritas::redis_commands::key_container keys{ key1, key2, key3 };
            const auto result = co_await key_commands.async_is_exists_many(keys);
            BOOST_CHECK_EQUAL(result, 2); // 应该有2个键存在

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_rename)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string old_key{ "test_key_old" };
            const std::string new_key{ "test_key_new" };
            const std::string value{ "test_value_rename" };
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ old_key, new_key });
            // 设置原键值对
            co_await string_commands.async_set(old_key, value);

            // 验证原键存在，新键不存在
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(old_key), true);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(new_key), false);

            // 重命名键
            const auto result = co_await key_commands.async_rename(old_key, new_key);
            BOOST_CHECK_EQUAL(result, true); // 重命名应该成功

            // 验证原键不存在，新键存在
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(old_key), false);
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(new_key), true);

            // 验证值被正确移动
            const auto retrieved_value = co_await string_commands.async_get(new_key);
            BOOST_REQUIRE(retrieved_value.has_value());
            BOOST_CHECK_EQUAL(retrieved_value.value(), value);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_type)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            const std::string key{ "test_key_type" };
            const std::string value{ "test_value_type" };

            // 设置一个字符串类型的键
            co_await string_commands.async_set(key, value);

            // 获取键的类型
            const auto type = co_await key_commands.async_get_type(key);
            BOOST_CHECK_EQUAL(type, "string"); // 应该是字符串类型

            // 检查不存在的键的类型
            const auto nonexistent_type = co_await key_commands.async_get_type("nonexistent_key");
            BOOST_CHECK_EQUAL(nonexistent_type, "none"); // 不存在的键类型应该是none

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_scan_all)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& key_commands = session->get_redis_key_commands();
            const auto& string_commands = session->get_redis_string_commands();

            // 设置一些测试键
            for (int i = 0; i < 5; ++i)
            {
                const std::string key = "test_scan_key_" + std::to_string(i);
                co_await string_commands.async_set(key, "value_" + std::to_string(i));
            }

            // 扫描匹配模式的键
            const std::string pattern = "test_scan_key_*";
            const auto result = co_await key_commands.async_scan_all(pattern);

            // 验证扫描结果至少包含我们设置的键
            BOOST_CHECK(result.size() >= 5);

            // 检查返回的键是否符合预期模式
            std::set<std::string> result_set(result.begin(), result.end());
            for (int i = 0; i < 5; ++i)
            {
                const std::string expected_key = "test_scan_key_" + std::to_string(i);
                BOOST_CHECK(result_set.count(session->get_prefixed_key(expected_key)) > 0);
            }

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()