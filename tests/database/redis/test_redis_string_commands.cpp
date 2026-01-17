#include "database/redis/redis_string_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/database_session_fixture.tpp"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

namespace
{
    [[nodiscard]] boost::asio::awaitable<void> check_async_set(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value{ "test_value" };

        const auto result = co_await string_commands.async_set(key, value);
        BOOST_CHECK_EQUAL(result, true);

        const auto get_result = co_await string_commands.async_get(key);
        BOOST_CHECK(get_result.has_value());
        BOOST_CHECK_EQUAL(get_result.value(), value);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_with_expire(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value{ "test_value_expire" };
        constexpr auto expire_seconds = 1;

        const auto result = co_await string_commands.async_set(key, value, expire_seconds);
        BOOST_CHECK_EQUAL(result, true);

        const auto get_result = co_await string_commands.async_get(key);
        BOOST_CHECK(get_result.has_value());
        BOOST_CHECK_EQUAL(get_result.value(), value);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_not_exists(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value1{ "test_value1" };
        const std::string value2{ "test_value2" };
        constexpr auto expire_seconds = 10;

        const auto result1 = co_await string_commands.async_set_not_exists(key, value1, expire_seconds);
        BOOST_CHECK_EQUAL(result1, true);

        const auto result2 = co_await string_commands.async_set_not_exists(key, value2, expire_seconds);
        BOOST_CHECK_EQUAL(result2, false);

        const auto get_result = co_await string_commands.async_get(key);
        BOOST_CHECK(get_result.has_value());
        BOOST_CHECK_EQUAL(get_result.value(), value1);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_exists(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value1{ "test_value1" };
        const std::string value2{ "test_value2" };

        const auto result1 = co_await string_commands.async_set_exists(key, value1);
        BOOST_CHECK_EQUAL(result1, false);

        co_await string_commands.async_set(key, value1);

        const auto result2 = co_await string_commands.async_set_exists(key, value2);
        BOOST_CHECK_EQUAL(result2, true);

        const auto get_result = co_await string_commands.async_get(key);
        BOOST_CHECK(get_result.has_value());
        BOOST_CHECK_EQUAL(get_result.value(), value2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& string_commands = session->get_redis_string_commands();
        celeritas::redis_string_commands::key_value_container key_values{ { "test_key1", "test_value1" },
                                                                          { "test_key2", "test_value2" },
                                                                          { "test_key3", "test_value3" } };

        const auto result = co_await string_commands.async_set_many(key_values);
        BOOST_CHECK_EQUAL(result, true);

        for (const auto& [key, value] : key_values)
        {
            const auto get_result = co_await string_commands.async_get(key);
            BOOST_CHECK(get_result.has_value());
            BOOST_CHECK_EQUAL(get_result.value(), value);
        }
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_append(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value1{ "test_value" };
        const std::string value2{ "_append" };

        co_await string_commands.async_set(key, value1);

        const auto result = co_await string_commands.async_append(key, value2);
        BOOST_CHECK_EQUAL(result, value1.length() + value2.length());

        const auto get_result = co_await string_commands.async_get(key);
        BOOST_CHECK(get_result.has_value());
        BOOST_CHECK_EQUAL(get_result.value(), value1 + value2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_increment_by(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        constexpr auto increment = 5;

        const auto result = co_await string_commands.async_increment_by(key, increment);
        BOOST_CHECK_EQUAL(result, increment);

        const auto result2 = co_await string_commands.async_increment_by(key, increment);
        BOOST_CHECK_EQUAL(result2, increment * 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_increment(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();

        const auto result = co_await string_commands.async_increment(key);
        BOOST_CHECK_EQUAL(result, 1);

        const auto result2 = co_await string_commands.async_increment(key);
        BOOST_CHECK_EQUAL(result2, 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_decrement_by(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        constexpr auto decrement = 3;

        co_await string_commands.async_set(key, "10");

        const auto result = co_await string_commands.async_decrement_by(key, decrement);
        BOOST_CHECK_EQUAL(result, 10 - decrement);

        const auto result2 = co_await string_commands.async_decrement_by(key, decrement);
        BOOST_CHECK_EQUAL(result2, 10 - decrement * 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_decrement(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();

        co_await string_commands.async_set(key, "5");

        const auto result = co_await string_commands.async_decrement(key);
        BOOST_CHECK_EQUAL(result, 4);

        const auto result2 = co_await string_commands.async_decrement(key);
        BOOST_CHECK_EQUAL(result2, 3);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value{ "test_value_get" };

        const auto result1 = co_await string_commands.async_get(key);
        BOOST_CHECK(!result1.has_value());

        co_await string_commands.async_set(key, value);

        const auto result2 = co_await string_commands.async_get(key);
        BOOST_CHECK(result2.has_value());
        BOOST_CHECK_EQUAL(result2.value(), value);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& string_commands = session->get_redis_string_commands();

        co_await string_commands.async_set("test_key_many1", "test_value_many1");
        co_await string_commands.async_set("test_key_many2", "test_value_many2");

        const celeritas::redis_string_commands::key_container keys{ "test_key_many1", "test_key_many2", "test_key_many3" };
        const auto result = co_await string_commands.async_get_many(keys);

        BOOST_CHECK_EQUAL(result.size(), keys.size());
        BOOST_CHECK_EQUAL(result[0], "test_value_many1");
        BOOST_CHECK_EQUAL(result[1], "test_value_many2");
        BOOST_CHECK_EQUAL(result[2], "");
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get_set(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value1{ "test_value1" };
        const std::string value2{ "test_value2" };

        const auto result1 = co_await string_commands.async_get_set(key, value1);
        BOOST_CHECK(!result1.has_value());

        const auto result2 = co_await string_commands.async_get_set(key, value2);
        BOOST_CHECK(result2.has_value());
        BOOST_CHECK_EQUAL(result2.value(), value1);

        const auto result3 = co_await string_commands.async_get(key);
        BOOST_CHECK(result3.has_value());
        BOOST_CHECK_EQUAL(result3.value(), value2);
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_string_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_set)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_with_expire)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_expire" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_with_expire(session, key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_not_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_nx" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_not_exists(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_xx" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_exists(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_set_many(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_append)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_append" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_append(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_incr_by" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_increment_by(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_incr" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_increment(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_decrement_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_decr_by" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_decrement_by(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_decrement)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_decr" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_decrement(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_get" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_get(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_get_many(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_set)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_key_get_set" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_get_set(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
