#include "database/redis/redis_sorted_set_commands.h"
#include "database/redis/sorted_set_member_score.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/test/unit_test.hpp>

namespace
{
    [[nodiscard]] boost::asio::awaitable<void> check_async_add_and_score(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::sorted_set_member_score member{ "member1", 10.5 };

        const auto add_count = co_await z_set_commands.async_add(key, member);
        BOOST_CHECK_EQUAL(add_count, 1);

        const auto score = co_await z_set_commands.async_score(key, "member1");
        BOOST_REQUIRE(score.has_value());
        BOOST_CHECK_CLOSE(*score, 10.5, 0.001);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_add_many_and_cardinality(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::redis_sorted_set_commands::sorted_set_member_score_container members{ { "member1", 1.0 },
                                                                                               { "member2", 2.0 } };

        const auto add_count = co_await z_set_commands.async_add_many(key, members);
        BOOST_CHECK_EQUAL(add_count, 2);

        const auto cardinality = co_await z_set_commands.async_sorted_set_cardinality(key);
        BOOST_CHECK_EQUAL(cardinality, 2);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_remove(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::sorted_set_member_score member{ "member1", 1.0 };

        co_await z_set_commands.async_add(key, member);

        const auto remove_count = co_await z_set_commands.async_remove(key, member);
        BOOST_CHECK_EQUAL(remove_count, 1);

        const auto cardinality = co_await z_set_commands.async_sorted_set_cardinality(key);
        BOOST_CHECK_EQUAL(cardinality, 0);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_remove_many(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::redis_sorted_set_commands::sorted_set_member_score_container members_to_add{ { "member1", 1.0 },
                                                                                                      { "member2", 2.0 } };
        const celeritas::redis_commands::key_container members_to_remove{ "member1", "member2" };

        co_await z_set_commands.async_add_many(key, members_to_add);

        const auto remove_count = co_await z_set_commands.async_remove_many(key, members_to_remove);
        BOOST_CHECK_EQUAL(remove_count, 2);

        const auto cardinality = co_await z_set_commands.async_sorted_set_cardinality(key);
        BOOST_CHECK_EQUAL(cardinality, 0);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_increment_by(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::sorted_set_member_score member{ "member1", 5.0 };

        co_await z_set_commands.async_add(key, member);

        const auto new_score = co_await z_set_commands.async_increment_by(key, 2.5, "member1");
        BOOST_REQUIRE(new_score.has_value());
        BOOST_CHECK_CLOSE(*new_score, 7.5, 0.001);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_range_and_reverse_range(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::redis_sorted_set_commands::sorted_set_member_score_container members{ { "one", 1 }, { "two", 2 }, { "three", 3 } };

        co_await z_set_commands.async_add_many(key, members);

        const auto range_result = co_await z_set_commands.async_range(key, 0, -1, true);
        BOOST_REQUIRE_EQUAL(range_result.size(), 3);
        BOOST_CHECK_EQUAL(range_result[0].get_member(), "one");
        BOOST_CHECK_CLOSE(range_result[0].get_score(), 1.0, 0.001);
        BOOST_CHECK_EQUAL(range_result[2].get_member(), "three");
        BOOST_CHECK_CLOSE(range_result[2].get_score(), 3.0, 0.001);

        const auto rev_range_result = co_await z_set_commands.async_reverse_range(key, 0, -1, true);
        BOOST_REQUIRE_EQUAL(rev_range_result.size(), 3);
        BOOST_CHECK_EQUAL(rev_range_result[0].get_member(), "three");
        BOOST_CHECK_CLOSE(rev_range_result[0].get_score(), 3.0, 0.001);
        BOOST_CHECK_EQUAL(rev_range_result[2].get_member(), "one");
        BOOST_CHECK_CLOSE(rev_range_result[2].get_score(), 1.0, 0.001);
    }

    [[nodiscard]] boost::asio::awaitable<void> check_async_rank_and_reverse_rank(const celeritas::redis_database_session_fixture::redis_database_session_shared_ptr& session, const std::string& key)
    {
        const auto& z_set_commands = session->get_redis_sorted_set_commands();
        const celeritas::redis_sorted_set_commands::sorted_set_member_score_container members{ { "one", 1 }, { "two", 2 }, { "three", 3 } };

        co_await z_set_commands.async_add_many(key, members);

        const auto rank = co_await z_set_commands.async_rank(key, "two");
        BOOST_REQUIRE(rank.has_value());
        BOOST_CHECK_EQUAL(*rank, 1);

        const auto reverse_rank = co_await z_set_commands.async_reverse_rank(key, "two");
        BOOST_REQUIRE(reverse_rank.has_value());
        BOOST_CHECK_EQUAL(*reverse_rank, 1);

        const auto reverse_rank_one = co_await z_set_commands.async_reverse_rank(key, "one");
        BOOST_REQUIRE(reverse_rank_one.has_value());
        BOOST_CHECK_EQUAL(*reverse_rank_one, 2);
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_sorted_set_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_add_and_score)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_z_set_add" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_add_and_score(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_add_many_and_cardinality)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_z_set_add_many" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_add_many_and_cardinality(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_z_set_remove" };

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

            const std::string key{ "test_z_set_remove_many" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_remove_many(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_z_set_incrby" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_increment_by(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_range_and_reverse_range)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const std::string key{ "test_z_set_range" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_range_and_reverse_range(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_rank_and_reverse_rank)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const std::string key{ "test_z_set_rank" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await check_async_rank_and_reverse_rank(session, key);
            co_await session->get_redis_key_commands().async_delete(key);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
