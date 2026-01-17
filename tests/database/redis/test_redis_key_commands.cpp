#include "database/redis/redis_key_commands.h"
#include "database/redis/redis_string_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <set>
#include <string>
#include <vector>

namespace
{
    [[nodiscard]] boost::asio::awaitable<void> check_async_delete(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key{ "test_key_delete" };
        const std::string value{ "test_value" };

        co_await string_commands.async_set(key, value);
        const auto exists_before = co_await key_commands.async_is_exists(key);
        BOOST_CHECK_EQUAL(exists_before, true);

        const auto result = co_await key_commands.async_delete(key);
        BOOST_CHECK_EQUAL(result, 1);

        const auto exists_after = co_await key_commands.async_is_exists(key);
        BOOST_CHECK_EQUAL(exists_after, false);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_delete_nonexistent_key(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();

        const std::string key{ "test_key_delete_nonexistent" };
        const auto result = co_await key_commands.async_delete(key);
        BOOST_CHECK_EQUAL(result, 0);
    }

    [[nodiscard]] boost::asio::awaitable<void> setup_keys_for_delete_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session,
                                                                          const celeritas::redis_commands::key_container& keys)
    {
        const auto& string_commands = session->get_redis_string_commands();
        const std::string value{ "test_value" };
        for (const auto& key : keys)
        {
            co_await string_commands.async_set(key, value);
        }
    }

    [[nodiscard]] boost::asio::awaitable<void> verify_keys_existence(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session,
                                                                     const celeritas::redis_commands::key_container& keys,
                                                                     const bool should_exist)
    {
        const auto& key_commands = session->get_redis_key_commands();
        for (const auto& key : keys)
        {
            BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(key), should_exist);
        }
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_delete_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();

        const celeritas::redis_commands::key_container keys{ "test_key_delete_many1",
                                                             "test_key_delete_many2",
                                                             "test_key_delete_many3" };

        co_await setup_keys_for_delete_many(session, keys);
        co_await verify_keys_existence(session, keys, true);

        const auto result = co_await key_commands.async_delete_many(keys);
        BOOST_CHECK_EQUAL(result, 3);

        co_await verify_keys_existence(session, keys, false);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_delete_many_partial_existence(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key1{ "test_key_delete_partial1" };
        const std::string key2{ "test_key_delete_partial2" };
        const std::string key3{ "test_key_delete_partial3" };
        const std::string value{ "test_value" };

        co_await string_commands.async_set(key1, value);
        co_await string_commands.async_set(key3, value);

        const celeritas::redis_commands::key_container keys{ key1, key2, key3 };
        const auto result = co_await key_commands.async_delete_many(keys);
        BOOST_CHECK_EQUAL(result, 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_expire_seconds(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key{ "test_key_expire" };
        const std::string value{ "test_value" };
        constexpr auto expire_seconds = 10;

        co_await string_commands.async_set(key, value);
        const auto result = co_await key_commands.async_set_expire_seconds(key, expire_seconds);
        BOOST_CHECK_EQUAL(result, true);

        const auto ttl = co_await key_commands.async_get_expire_seconds(key);
        BOOST_CHECK(0 < ttl && ttl <= expire_seconds);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get_expire_seconds(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key1{ "test_key_ttl1" };
        const std::string key2{ "test_key_ttl2" };
        const std::string value{ "test_value" };
        constexpr auto expire_seconds = 10;

        co_await string_commands.async_set(key1, value, -1);
        co_await string_commands.async_set(key2, value, expire_seconds);

        const auto ttl1 = co_await key_commands.async_get_expire_seconds(key1);
        BOOST_CHECK_EQUAL(ttl1, -1);

        const auto ttl2 = co_await key_commands.async_get_expire_seconds(key2);
        BOOST_CHECK(0 < ttl2 && ttl2 <= expire_seconds);

        const auto ttl3 = co_await key_commands.async_get_expire_seconds("nonexistent_key");
        BOOST_CHECK_EQUAL(ttl3, -2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_is_exists(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key{ "test_key_exists" };
        const std::string value{ "test_value" };
        co_await session->get_redis_key_commands().async_delete(key);

        const auto exists1 = co_await key_commands.async_is_exists(key);
        BOOST_CHECK_EQUAL(exists1, false);

        co_await string_commands.async_set(key, value);

        const auto exists2 = co_await key_commands.async_is_exists(key);
        BOOST_CHECK_EQUAL(exists2, true);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_is_exists_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key1{ "test_key_exists_many1" };
        const std::string key2{ "test_key_exists_many2" };
        const std::string key3{ "test_key_exists_many3" };
        const std::string value{ "test_value" };
        co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2, key3 });

        co_await string_commands.async_set(key1, value);
        co_await string_commands.async_set(key3, value);

        const celeritas::redis_commands::key_container keys{ key1, key2, key3 };
        const auto result = co_await key_commands.async_is_exists_many(keys);
        BOOST_CHECK_EQUAL(result, 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> setup_rename_keys(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session,
                                                                 const std::string& old_key,
                                                                 const std::string& new_key,
                                                                 const std::string& value)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        co_await key_commands.async_delete_many(std::vector{ old_key, new_key });
        co_await string_commands.async_set(old_key, value);

        BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(old_key), true);
        BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(new_key), false);
    }

    [[nodiscard]] boost::asio::awaitable<void> verify_rename(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session,
                                                             const std::string& old_key,
                                                             const std::string& new_key,
                                                             const std::string& expected_value)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(old_key), false);
        BOOST_CHECK_EQUAL(co_await key_commands.async_is_exists(new_key), true);

        const auto retrieved_value = co_await string_commands.async_get(new_key);
        BOOST_REQUIRE(retrieved_value.has_value());
        BOOST_CHECK_EQUAL(retrieved_value.value(), expected_value);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_rename(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();

        const std::string old_key{ "test_key_old" };
        const std::string new_key{ "test_key_new" };
        const std::string value{ "test_value_rename" };

        co_await setup_rename_keys(session, old_key, new_key, value);

        const auto result = co_await key_commands.async_rename(old_key, new_key);
        BOOST_CHECK_EQUAL(result, true);

        co_await verify_rename(session, old_key, new_key, value);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get_type(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();
        const auto& string_commands = session->get_redis_string_commands();

        const std::string key{ "test_key_type" };
        const std::string value{ "test_value_type" };

        co_await string_commands.async_set(key, value);

        const auto type = co_await key_commands.async_get_type(key);
        BOOST_CHECK_EQUAL(type, "string");

        const auto nonexistent_type = co_await key_commands.async_get_type("nonexistent_key");
        BOOST_CHECK_EQUAL(nonexistent_type, "none");
    }

    constexpr auto loop_count = 5;

    [[nodiscard]] boost::asio::awaitable<void> setup_scan_keys(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& string_commands = session->get_redis_string_commands();
        for (auto i = 0; i < loop_count; ++i)
        {
            const auto key = "test_scan_key_" + std::to_string(i);
            co_await string_commands.async_set(key, "value_" + std::to_string(i));
        }
    }

    void verify_scan_results(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::vector<std::string>& result)
    {
        BOOST_CHECK(result.size() >= loop_count);
        const std::set result_set(result.begin(), result.end());
        for (auto i = 0; i < loop_count; ++i)
        {
            const auto expected_key = "test_scan_key_" + std::to_string(i);
            BOOST_CHECK(result_set.contains(session->get_prefixed_key(expected_key)));
        }
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_scan_all(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& key_commands = session->get_redis_key_commands();

        co_await setup_scan_keys(session);

        const std::string pattern = "test_scan_key_*";
        const auto result = co_await key_commands.async_scan_all(pattern);

        verify_scan_results(session, result);
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_key_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_delete)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_delete(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_nonexistent_key)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_delete_nonexistent_key(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_delete_many(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_many_partial_existence)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_delete_many_partial_existence(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_expire_seconds)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_set_expire_seconds(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_expire_seconds)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_get_expire_seconds(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_is_exists)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_is_exists(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_is_exists_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_is_exists_many(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_rename)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_rename(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_type)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_get_type(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_scan_all)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            co_await check_async_scan_all(session);
            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
