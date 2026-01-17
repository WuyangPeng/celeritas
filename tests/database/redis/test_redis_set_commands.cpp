#include "database/redis/redis_key_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <set>

namespace
{
    [[nodiscard]] boost::asio::awaitable<void> check_async_add(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string member{ "member1" };

        const auto result = co_await set_commands.async_add(key, member);
        BOOST_CHECK_EQUAL(result, 1);

        const auto result2 = co_await set_commands.async_add(key, member);
        BOOST_CHECK_EQUAL(result2, 0);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_add_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const celeritas::redis_commands::key_container members{ "member1", "member2", "member3", "member1" };

        const auto result = co_await set_commands.async_add_many(key, members);
        BOOST_CHECK_EQUAL(result, 3);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_remove(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string member{ "member_to_remove" };

        co_await set_commands.async_add(key, member);

        const auto result = co_await set_commands.async_remove(key, member);
        BOOST_CHECK_EQUAL(result, 1);

        const auto result2 = co_await set_commands.async_remove(key, member);
        BOOST_CHECK_EQUAL(result2, 0);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_remove_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

        co_await set_commands.async_add_many(key, members);

        const celeritas::redis_commands::key_container members_to_remove{ "member1", "member2", "nonexistent_member" };
        const auto result = co_await set_commands.async_remove_many(key, members_to_remove);
        BOOST_CHECK_EQUAL(result, 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_cardinality(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

        const auto result1 = co_await set_commands.async_set_cardinality(key);
        BOOST_CHECK_EQUAL(result1, 0);

        co_await set_commands.async_add_many(key, members);

        const auto result2 = co_await set_commands.async_set_cardinality(key);
        BOOST_CHECK_EQUAL(result2, 3);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_is_member(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string member{ "test_member" };

        const auto result1 = co_await set_commands.async_set_is_member(key, member);
        BOOST_CHECK_EQUAL(result1, false);

        co_await set_commands.async_add(key, member);

        const auto result2 = co_await set_commands.async_set_is_member(key, member);
        BOOST_CHECK_EQUAL(result2, true);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_members(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

        co_await set_commands.async_add_many(key, members);

        const auto result = co_await set_commands.async_set_members(key);
        BOOST_CHECK_EQUAL(result.size(), 3);

        std::set<std::string> result_set;
        for (const auto& item : result)
        {
            result_set.insert(item);
        }

        for (const auto& member : static_cast<const std::vector<std::string>&>(members))
        {
            BOOST_CHECK(result_set.contains(member));
        }
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_union(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string key1{ "test_set_union1" };
        const std::string key2{ "test_set_union2" };
        const celeritas::redis_commands::key_container members1{ "member1", "member2" };
        const celeritas::redis_commands::key_container members2{ "member2", "member3" };

        co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });
        co_await set_commands.async_add_many(key1, members1);
        co_await set_commands.async_add_many(key2, members2);

        const celeritas::redis_commands::key_container keys{ key1, key2 };
        const auto result = co_await set_commands.async_set_union(keys);
        BOOST_CHECK_EQUAL(result.size(), 3);

        std::set<std::string> result_set;
        for (const auto& item : result)
        {
            result_set.insert(item);
        }

        BOOST_CHECK(result_set.contains("member1"));
        BOOST_CHECK(result_set.contains("member2"));
        BOOST_CHECK(result_set.contains("member3"));
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_inter(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string key1{ "test_set_inter1" };
        const std::string key2{ "test_set_inter2" };
        const celeritas::redis_commands::key_container members1{ "member1", "member2", "member3" };
        const celeritas::redis_commands::key_container members2{ "member2", "member3", "member4" };

        co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });
        co_await set_commands.async_add_many(key1, members1);
        co_await set_commands.async_add_many(key2, members2);

        const celeritas::redis_commands::key_container keys{ key1, key2 };
        const auto result = co_await set_commands.async_set_inter(keys);
        BOOST_CHECK_EQUAL(result.size(), 2);

        std::set<std::string> result_set;
        for (const auto& item : result)
        {
            result_set.insert(item);
        }

        BOOST_CHECK(result_set.contains("member2"));
        BOOST_CHECK(result_set.contains("member3"));
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_set_diff(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session)
    {
        const auto& set_commands = session->get_redis_set_commands();
        const std::string key1{ "test_set_diff1" };
        const std::string key2{ "test_set_diff2" };
        const celeritas::redis_commands::key_container members1{ "member1", "member2", "member3" };
        const celeritas::redis_commands::key_container members2{ "member2", "member4" };

        co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });
        co_await set_commands.async_add_many(key1, members1);
        co_await set_commands.async_add_many(key2, members2);

        const celeritas::redis_commands::key_container keys{ key1, key2 };
        const auto result = co_await set_commands.async_set_diff(keys);
        BOOST_CHECK_EQUAL(result.size(), 2);

        std::set<std::string> result_set;
        for (const auto& item : result)
        {
            result_set.insert(item);
        }

        BOOST_CHECK(result_set.contains("member1"));
        BOOST_CHECK(result_set.contains("member3"));
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_set_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_add)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_add" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_add(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_add_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_add_many" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_add_many(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_remove" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_remove(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_remove_many" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_remove_many(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_cardinality)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_cardinality" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_cardinality(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_is_member)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_is_member" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_is_member(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_members)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_set_members" };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_set_members(session, key);
            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_union)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_set_union(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_inter)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_set_inter(session);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_diff)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            co_await check_async_set_diff(session);
            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
