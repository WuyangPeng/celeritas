#include "database/redis/redis_hash_commands.h"
#include "database/session/fixture/redis_database_session_fixture.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(redis_hash_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_async_set_and_get)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_set_get" };
            const std::string field{ "field1" };
            const std::string value{ "value1" };

            co_await session->get_redis_key_commands().async_delete(key);

            const auto result = co_await hash_commands.async_set(key, field, value);
            BOOST_CHECK_EQUAL(result, 1);

            const auto retrieved_value = co_await hash_commands.async_get(key, field);
            BOOST_REQUIRE(retrieved_value.has_value());
            BOOST_CHECK_EQUAL(*retrieved_value, value);

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_set_many_and_get_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_set_many" };
            const celeritas::redis_commands::key_value_container field_values{
                { "f1", "v1" }, { "f2", "v2" }
            };

            co_await session->get_redis_key_commands().async_delete(key);

            const auto success = co_await hash_commands.async_set_many(key, field_values);
            BOOST_CHECK(success);

            const celeritas::redis_commands::key_container fields{ "f1", "f2", "non_existent" };
            const auto values = co_await hash_commands.async_get_many(key, fields);
            BOOST_REQUIRE_EQUAL(values.size(), 3);
            BOOST_CHECK_EQUAL(values[0], "v1");
            BOOST_CHECK_EQUAL(values[1], "v2");
            BOOST_CHECK(values[2].empty());

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_delete_and_delete_many)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_del" };
            const celeritas::redis_commands::key_value_container fv{ { "f1", "v1" }, { "f2", "v2" }, { "f3", "v3" } };
            co_await session->get_redis_key_commands().async_delete(key);
            co_await hash_commands.async_set_many(key, fv);

            const auto del_count1 = co_await hash_commands.async_delete(key, "f1");
            BOOST_CHECK_EQUAL(del_count1, 1);

            const auto del_count_many = co_await hash_commands.async_delete_many(key, { "f2", "f3" });
            BOOST_CHECK_EQUAL(del_count_many, 2);

            const auto all_fields = co_await hash_commands.async_get_fields(key);
            BOOST_CHECK(all_fields.empty());

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_increment_by)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_incr" };
            const std::string field{ "counter" };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await hash_commands.async_set(key, field, "10");

            const auto new_value = co_await hash_commands.async_increment_by(key, field, 5);
            BOOST_CHECK_EQUAL(new_value, 15);

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_all_and_get_fields)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_get_all" };
            const celeritas::redis_commands::key_value_container fv{ { "f1", "v1" }, { "f2", "v2" } };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await hash_commands.async_set_many(key, fv);

            const auto all_values = co_await hash_commands.async_get_all(key);
            BOOST_CHECK(!all_values.empty());
            BOOST_REQUIRE_EQUAL(all_values.size(), 2);
            BOOST_CHECK_EQUAL(all_values.at("f1"), "v1");
            BOOST_CHECK_EQUAL(all_values.at("f2"), "v2");

            const auto all_fields = co_await hash_commands.async_get_fields(key);
            BOOST_REQUIRE_EQUAL(all_fields.size(), 2);

            BOOST_CHECK(std::ranges::find(all_fields, "f1") != all_fields.cend());
            BOOST_CHECK(std::ranges::find(all_fields, "f2") != all_fields.cend());

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_get_all_by_real_key)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();
            const auto& hash_commands = session->get_redis_hash_commands();
            const std::string key{ "test_hash_get_all_real" };
            const auto real_key{ session->get_prefixed_key(key) };
            const celeritas::redis_commands::key_value_container fv{ { "f1", "v1" }, { "f2", "v2" } };

            co_await session->get_redis_key_commands().async_delete(key);
            co_await hash_commands.async_set_many(key, fv);

            const auto all_values = co_await hash_commands.async_get_all_by_real_key(real_key);
            BOOST_CHECK(!all_values.empty());
            BOOST_REQUIRE_EQUAL(all_values.size(), 2);
            BOOST_CHECK_EQUAL(all_values.at("f1"), "v1");
            BOOST_CHECK_EQUAL(all_values.at("f2"), "v2");

            co_await session->get_redis_key_commands().async_delete(key);
            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
