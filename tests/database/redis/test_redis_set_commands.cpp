#include "database/redis/redis_key_commands.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <set>

BOOST_FIXTURE_TEST_SUITE(redis_set_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_add)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_add" };
            const std::string member{ "member1" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 测试添加新成员
            const auto result = co_await set_commands.async_add(key, member);
            BOOST_CHECK_EQUAL(result, 1); // 应该添加了1个新成员

            // 测试添加相同成员（应该不会增加计数）
            const auto result2 = co_await set_commands.async_add(key, member);
            BOOST_CHECK_EQUAL(result2, 0); // 没有新增成员

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_add_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_add_many" };
            const celeritas::redis_commands::key_container members{ "member1", "member2", "member3", "member1" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 测试添加多个成员（包含重复项）
            const auto result = co_await set_commands.async_add_many(key, members);
            BOOST_CHECK_EQUAL(result, 3); // 应该添加了3个新成员（去重后）

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_remove" };
            const std::string member{ "member_to_remove" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 先添加一个成员
            co_await set_commands.async_add(key, member);

            // 测试移除存在的成员
            const auto result = co_await set_commands.async_remove(key, member);
            BOOST_CHECK_EQUAL(result, 1); // 成功移除了1个成员

            // 测试移除不存在的成员
            const auto result2 = co_await set_commands.async_remove(key, member);
            BOOST_CHECK_EQUAL(result2, 0); // 没有可移除的成员

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_remove_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_remove_many" };
            const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 添加多个成员
            co_await set_commands.async_add_many(key, members);

            // 准备要删除的成员列表（包含存在的和不存在的）
            const celeritas::redis_commands::key_container members_to_remove{ "member1", "member2", "nonexistent_member" };

            // 测试移除多个成员
            const auto result = co_await set_commands.async_remove_many(key, members_to_remove);
            BOOST_CHECK_EQUAL(result, 2); // 成功移除了2个成员

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_cardinality)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_cardinality" };
            const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 测试空集合的基数
            const auto result1 = co_await set_commands.async_set_cardinality(key);
            BOOST_CHECK_EQUAL(result1, 0);

            // 添加成员
            co_await set_commands.async_add_many(key, members);

            // 测试非空集合的基数
            const auto result2 = co_await set_commands.async_set_cardinality(key);
            BOOST_CHECK_EQUAL(result2, 3);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_is_member)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_is_member" };
            const std::string member{ "test_member" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 测试不存在的成员
            const auto result1 = co_await set_commands.async_set_is_member(key, member);
            BOOST_CHECK_EQUAL(result1, false);

            // 添加成员
            co_await set_commands.async_add(key, member);

            // 测试存在的成员
            const auto result2 = co_await set_commands.async_set_is_member(key, member);
            BOOST_CHECK_EQUAL(result2, true);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_members)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key{ "test_set_members" };
            const celeritas::redis_commands::key_container members{ "member1", "member2", "member3" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key });

            // 添加成员
            co_await set_commands.async_add_many(key, members);

            // 获取所有成员
            const auto result = co_await set_commands.async_set_members(key);

            // 验证返回的成员数量
            BOOST_CHECK_EQUAL(result.size(), 3);

            // 验证返回的成员内容
            std::set<std::string> result_set;
            for (const auto& item : result)
            {
                result_set.insert(item);
            }

            for (const auto& member : static_cast<const std::vector<std::string>&>(members))
            {
                BOOST_CHECK(result_set.contains(member));
            }

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_union)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key1{ "test_set_union1" };
            const std::string key2{ "test_set_union2" };

            const celeritas::redis_commands::key_container members1{ "member1", "member2" };
            const celeritas::redis_commands::key_container members2{ "member2", "member3" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });

            // 添加成员到两个集合
            co_await set_commands.async_add_many(key1, members1);
            co_await set_commands.async_add_many(key2, members2);

            // 获取两个集合的并集
            const celeritas::redis_commands::key_container keys{ key1, key2 };
            const auto result = co_await set_commands.async_set_union(keys);

            // 并集应该包含所有唯一成员：member1, member2, member3
            BOOST_CHECK_EQUAL(result.size(), 3);

            std::set<std::string> result_set;
            for (const auto& item : result)
            {
                result_set.insert(item);
            }

            BOOST_CHECK(result_set.contains("member1"));
            BOOST_CHECK(result_set.contains("member2"));
            BOOST_CHECK(result_set.contains("member3"));

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_inter)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key1{ "test_set_inter1" };
            const std::string key2{ "test_set_inter2" };

            const celeritas::redis_commands::key_container members1{ "member1", "member2", "member3" };
            const celeritas::redis_commands::key_container members2{ "member2", "member3", "member4" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });

            // 添加成员到两个集合
            co_await set_commands.async_add_many(key1, members1);
            co_await set_commands.async_add_many(key2, members2);

            // 获取两个集合的交集
            const celeritas::redis_commands::key_container keys{ key1, key2 };
            const auto result = co_await set_commands.async_set_inter(keys);

            // 交集应该只包含共同成员：member2, member3
            BOOST_CHECK_EQUAL(result.size(), 2);

            std::set<std::string> result_set;
            for (const auto& item : result)
            {
                result_set.insert(item);
            }

            BOOST_CHECK(result_set.contains("member2"));
            BOOST_CHECK(result_set.contains("member3"));

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_diff)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& set_commands = session->get_redis_set_commands();

            const std::string key1{ "test_set_diff1" };
            const std::string key2{ "test_set_diff2" };

            const celeritas::redis_commands::key_container members1{ "member1", "member2", "member3" };
            const celeritas::redis_commands::key_container members2{ "member2", "member4" };

            // 清理可能存在的旧数据
            co_await session->get_redis_key_commands().async_delete_many(std::vector{ key1, key2 });

            // 添加成员到两个集合
            co_await set_commands.async_add_many(key1, members1);
            co_await set_commands.async_add_many(key2, members2);

            // 获取两个集合的差集（key1 - key2）
            const celeritas::redis_commands::key_container keys{ key1, key2 };
            const auto result = co_await set_commands.async_set_diff(keys);

            // 差集应该包含在key1中但不在key2中的成员：member1, member3
            BOOST_CHECK_EQUAL(result.size(), 2);

            std::set<std::string> result_set;
            for (const auto& item : result)
            {
                result_set.insert(item);
            }

            BOOST_CHECK(result_set.contains("member1"));
            BOOST_CHECK(result_set.contains("member3"));

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()