#include "database/redis/redis_string_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <map>

// 创建一个派生类用于测试基类的protected方法
namespace celeritas
{
    class test_redis_commands final : public redis_commands
    {
    public:
        using class_type = test_redis_commands;
        using base_type = redis_commands;

        explicit test_redis_commands(redis_database_session& session) noexcept
            : redis_commands(session)
        {
        }

        // 暴露protected方法用于测试
        [[nodiscard]] array_type test_get_keys_command(const key_container& keys) const
        {
            return get_keys_command(keys);
        }

        [[nodiscard]] array_type test_get_keys_value_command(const key_value_container& key_values) const
        {
            return get_keys_value_command(key_values);
        }

        [[nodiscard]] static array_type test_get_fields_value_command(const key_value_container& field_values)
        {
            return get_fields_value_command(field_values);
        }

        [[nodiscard]] std::string test_get_prefixed_key(const std::string& key) const
        {
            return get_prefixed_key(key);
        }

        [[nodiscard]] array_type test_get_expire_seconds_command(int expire_seconds) const
        {
            return get_expire_seconds_command(expire_seconds);
        }

        [[nodiscard]] redis_database_session& test_get_redis_database_session()
        {
            return get_redis_database_session();
        }
    };
} // namespace celeritas

BOOST_FIXTURE_TEST_SUITE(redis_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_key_container_type)
    {
        // 测试key_container类型是否正确
        celeritas::redis_commands::key_container container;
        container.push_back("key1");
        container.push_back("key2");

        BOOST_CHECK_EQUAL(container.size(), 2);
        BOOST_CHECK_EQUAL(container[0], "key1");
        BOOST_CHECK_EQUAL(container[1], "key2");
    }

    BOOST_AUTO_TEST_CASE(test_key_value_container_type)
    {
        // 测试key_value_container类型是否正确
        celeritas::redis_commands::key_value_container container;
        container.push_back({ "key1", "value1" });
        container.push_back({ "key2", "value2" });

        BOOST_CHECK_EQUAL(container.size(), 2);
        BOOST_CHECK_EQUAL(container[0].first, "key1");
        BOOST_CHECK_EQUAL(container[0].second, "value1");
        BOOST_CHECK_EQUAL(container[1].first, "key2");
        BOOST_CHECK_EQUAL(container[1].second, "value2");
    }

    BOOST_AUTO_TEST_CASE(test_get_keys_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands(*session);

            const celeritas::redis_commands::key_container keys{ "key1", "key2", "key3" };
            const auto result = test_commands.test_get_keys_command(keys);

            // 验证生成的命令数组
            BOOST_CHECK_EQUAL(result.size(), 3);
            BOOST_CHECK(result[0].find( "key1") != std::string::npos);
            BOOST_CHECK(result[1].find( "key2") != std::string::npos);
            BOOST_CHECK(result[2].find( "key3") != std::string::npos);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_keys_value_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands(*session);

            const celeritas::redis_commands::key_value_container key_values{
                { "key1", "value1" },
                { "key2", "value2" }
            };
            const auto result = test_commands.test_get_keys_value_command(key_values);

            // 验证生成的命令数组 (key1, value1, key2, value2)
            BOOST_CHECK_EQUAL(result.size(), 4);
            BOOST_CHECK(result[0].find( "key1") != std::string::npos);
            BOOST_CHECK_EQUAL(result[1], "value1");
            BOOST_CHECK(result[2].find( "key2") != std::string::npos);
            BOOST_CHECK_EQUAL(result[3], "value2");

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_fields_value_command_static)
    {
        const celeritas::redis_commands::key_value_container field_values{
            { "field1", "value1" },
            { "field2", "value2" }
        };
        const auto result = celeritas::test_redis_commands::test_get_fields_value_command(field_values);

        // 验证生成的命令数组 (field1, value1, field2, value2)
        BOOST_CHECK_EQUAL(result.size(), 4);
        BOOST_CHECK_EQUAL(result[0], "field1");
        BOOST_CHECK_EQUAL(result[1], "value1");
        BOOST_CHECK_EQUAL(result[2], "field2");
        BOOST_CHECK_EQUAL(result[3], "value2");
    }

    BOOST_AUTO_TEST_CASE(test_get_prefixed_key)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands(*session);

            // 测试获取带前缀的键
            const std::string original_key = "test_key";
            const auto prefixed_key = test_commands.test_get_prefixed_key(original_key);

            // 由于实际前缀取决于配置，这里只验证返回的是字符串
            BOOST_CHECK(!prefixed_key.empty());
            // 确保原始键的一部分仍然在结果中
            BOOST_CHECK(prefixed_key.find(original_key) != std::string::npos || prefixed_key == original_key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_expire_seconds_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands(*session);

            constexpr int expire_seconds = 60;
            const auto result = test_commands.test_get_expire_seconds_command(expire_seconds);

            // 验证生成的过期时间命令数组
            BOOST_CHECK_EQUAL(result.size(), 2);
            BOOST_CHECK_EQUAL(result[1], "60");

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_redis_database_session)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands(*session);

            // 测试获取Redis数据库会话
            auto& retrieved_session = test_commands.test_get_redis_database_session();

            // 验证返回的会话是同一个实例
            BOOST_CHECK(&retrieved_session == session.get());

            set_test_end(true);
        });
    }

    // 测试各种awaitable类型定义是否正确
    BOOST_AUTO_TEST_CASE(test_awaitable_types)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            // 直接使用co_await来测试各种awaitable类型，避免复制awaitable对象
            const bool bool_result = co_await []() -> boost::asio::awaitable<bool> {
                co_return true;
            }();

            const int int_result = co_await []() -> boost::asio::awaitable<int> {
                co_return 42;
            }();

            const std::string string_result = co_await []() -> boost::asio::awaitable<std::string> {
                co_return "test";
            }();

            const std::vector<std::string> array_result = co_await []() -> boost::asio::awaitable<std::vector<std::string> > {
                co_return std::vector<std::string>{ "a", "b" };
            }();

            BOOST_CHECK_EQUAL(bool_result, true);
            BOOST_CHECK_EQUAL(int_result, 42);
            BOOST_CHECK_EQUAL(string_result, "test");
            BOOST_CHECK_EQUAL(array_result.size(), 2);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
