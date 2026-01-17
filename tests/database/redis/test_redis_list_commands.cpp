#include "database/redis/redis_list_commands.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/test/unit_test.hpp>

namespace
{
    [[nodiscard]] boost::asio::awaitable<void> check_async_left_push_and_pop(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& list_commands = session->get_redis_list_commands();

        const auto length = co_await list_commands.async_left_push(key, "value1");
        BOOST_CHECK_EQUAL(length, 1);

        auto popped = co_await list_commands.async_left_pop(key);
        BOOST_REQUIRE(popped.has_value());
        BOOST_CHECK_EQUAL(*popped, "value1");
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_right_push_and_pop(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& list_commands = session->get_redis_list_commands();

        const auto length = co_await list_commands.async_right_push(key, "value1");
        BOOST_CHECK_EQUAL(length, 1);

        const auto popped = co_await list_commands.async_right_pop(key);
        BOOST_REQUIRE(popped.has_value());
        BOOST_CHECK_EQUAL(*popped, "value1");
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_push_many_and_get_length(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& list_commands = session->get_redis_list_commands();
        const celeritas::redis_commands::key_container values{ "v1", "v2", "v3" };

        const auto l_length = co_await list_commands.async_left_push_many(key, values);
        BOOST_CHECK_EQUAL(l_length, 3);

        const auto r_length = co_await list_commands.async_right_push_many(key, values);
        BOOST_CHECK_EQUAL(r_length, 6);

        const auto total_length = co_await list_commands.async_get_length(key);
        BOOST_CHECK_EQUAL(total_length, 6);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_get_range(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& list_commands = session->get_redis_list_commands();
        const celeritas::redis_commands::key_container values = { "v1", "v2", "v3" };

        co_await list_commands.async_right_push_many(key, values);

        const auto range = co_await list_commands.async_get_range(key, 0, -1);
        BOOST_REQUIRE_EQUAL(range.size(), 3);
        BOOST_CHECK_EQUAL(range[0], "v1");
        BOOST_CHECK_EQUAL(range[2], "v3");
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_remove(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& list_commands = session->get_redis_list_commands();
        const celeritas::redis_commands::key_container values{ "a", "b", "a", "c", "a" };

        co_await list_commands.async_right_push_many(key, values);

        const auto removed_count = co_await list_commands.async_remove(key, 2, "a");
        BOOST_CHECK_EQUAL(removed_count, 2);

        const auto length = co_await list_commands.async_get_length(key);
        BOOST_CHECK_EQUAL(length, 3);

        const auto range = co_await list_commands.async_get_range(key, 0, -1);
        BOOST_REQUIRE_EQUAL(range.size(), 3);
        BOOST_CHECK_EQUAL(range[0], "b");
        BOOST_CHECK_EQUAL(range[1], "c");
        BOOST_CHECK_EQUAL(range[2], "a");
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_blocking_left_pop(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& list_commands = session->get_redis_list_commands();
        const std::string key1{ "test_list_blpop1" };
        const std::string key2{ "test_list_blpop2" };

        co_await session->get_redis_key_commands().async_delete(key1);
        co_await session->get_redis_key_commands().async_delete(key2);
        const auto result_timeout = co_await list_commands.async_blocking_left_pop({ key1 }, 1);
        BOOST_CHECK(!result_timeout.has_value());

        co_await list_commands.async_left_push(key1, "val1");
        const auto result_pop = co_await list_commands.async_blocking_left_pop({ key1, key2 }, 1);
        BOOST_REQUIRE(result_pop.has_value());
        BOOST_CHECK_EQUAL(result_pop->first, session->get_prefixed_key(key1));
        BOOST_CHECK_EQUAL(result_pop->second, "val1");

        co_await session->get_redis_key_commands().async_delete(key1);
        co_await session->get_redis_key_commands().async_delete(key2);
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_list_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_left_push_and_pop)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_list_lpush" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_left_push_and_pop(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_right_push_and_pop)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_list_rpush" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_right_push_and_pop(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_push_many_and_get_length)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_list_push_many" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_push_many_and_get_length(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_range)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_list_range" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_get_range(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_list_remove" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_remove(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_blocking_left_pop)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_blocking_left_pop(session);
            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
