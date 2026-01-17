#include "database/detail/check_helper.h"
#include "database/session/redis_database_session.h"
#include "database/session/fixture/redis_database_session_fixture.h"
#include "detail/test_redis_commands.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_FIXTURE_TEST_SUITE(redis_commands_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_key_container_type)
    {
        const celeritas::redis_commands::key_container container{ "key1", "key2" };

        BOOST_CHECK_EQUAL(container.size(), 2);
        BOOST_CHECK_EQUAL(container.at(0), "key1");
        BOOST_CHECK_EQUAL(container.at(1), "key2");
    }

    BOOST_AUTO_TEST_CASE(test_key_value_container_type)
    {
        const celeritas::redis_commands::key_value_container container{ { "key1", "value1" }, { "key2", "value2" } };

        BOOST_CHECK_EQUAL(container.size(), 2);
        BOOST_CHECK_EQUAL(container.at(0).first, "key1");
        BOOST_CHECK_EQUAL(container.at(0).second, "value1");
        BOOST_CHECK_EQUAL(container.at(1).first, "key2");
        BOOST_CHECK_EQUAL(container.at(1).second, "value2");
    }

    BOOST_AUTO_TEST_CASE(test_get_keys_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const celeritas::test_redis_commands test_commands{ *session };

            const celeritas::redis_commands::key_container keys{ "key1", "key2", "key3" };
            const auto result = test_commands.test_get_keys_command(keys);

            BOOST_CHECK_EQUAL(result.size(), 3);
            BOOST_CHECK(result.at(0).find( "key1") != std::string::npos);
            BOOST_CHECK(result.at(1).find( "key2") != std::string::npos);
            BOOST_CHECK(result.at(2).find( "key3") != std::string::npos);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_keys_value_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const celeritas::test_redis_commands test_commands{ *session };

            const celeritas::redis_commands::key_value_container key_values{ { "key1", "value1" }, { "key2", "value2" } };

            const auto result = test_commands.test_get_keys_value_command(key_values);

            BOOST_CHECK_EQUAL(result.size(), 4);
            BOOST_CHECK(result.at(0).find( "key1") != std::string::npos);
            BOOST_CHECK_EQUAL(result.at(1), "value1");
            BOOST_CHECK(result.at(2).find( "key2") != std::string::npos);
            BOOST_CHECK_EQUAL(result.at(3), "value2");

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_fields_value_command_static)
    {
        const celeritas::redis_commands::key_value_container field_values{ { "field1", "value1" },
                                                                           { "field2", "value2" } };

        const auto result = celeritas::test_redis_commands::test_get_fields_value_command(field_values);

        BOOST_CHECK_EQUAL(result.size(), 4);
        BOOST_CHECK_EQUAL(result.at(0), "field1");
        BOOST_CHECK_EQUAL(result.at(1), "value1");
        BOOST_CHECK_EQUAL(result.at(2), "field2");
        BOOST_CHECK_EQUAL(result.at(3), "value2");
    }

    BOOST_AUTO_TEST_CASE(test_get_prefixed_key)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const celeritas::test_redis_commands test_commands{ *session };

            const std::string original_key{ "test_key" };
            const auto prefixed_key = test_commands.test_get_prefixed_key(original_key);

            BOOST_CHECK(!prefixed_key.empty());

            BOOST_CHECK(prefixed_key.find(original_key) != std::string::npos || prefixed_key == original_key);

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_expire_seconds_command)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            const celeritas::test_redis_commands test_commands{ *session };

            constexpr auto expire_seconds = 60;
            const auto result = test_commands.test_get_expire_seconds_command(expire_seconds);

            BOOST_CHECK_EQUAL(result.size(), 2);
            BOOST_CHECK_EQUAL(result.at(1), "60");

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_redis_database_session)
    {
        run([this]() -> boost::asio::awaitable<void> {
            const auto session = get_session();
            co_await session->async_connect();

            celeritas::test_redis_commands test_commands{ *session };

            const auto& retrieved_session = test_commands.test_get_redis_database_session();

            BOOST_CHECK(&retrieved_session == session.get());

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
