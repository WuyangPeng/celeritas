#include "config/basic/database_type.h"
#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.tpp"
#include "database/document/test/logs_data.h"
#include "database/document/test/properties_data.h"
#include "database/generated/redis/test/redis_test.h"
#include "database/session/redis_database_session.h"
#include "detail/check_helper.h"
#include "fixture/redis_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <map>
#include <memory>
#include <vector>

namespace
{
    constexpr auto user_id = 123456789;

    [[nodiscard]] boost::asio::awaitable<void> test_insert(const celeritas::redis_database_session_fixture& fixture, celeritas::redis_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
        entity.clear_modify();
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_one(const celeritas::redis_database_session_fixture& fixture, const std::string& expected_chapter_name, const int64_t expected_currency)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::redis_test::get_select(celeritas::database_type::redis, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::redis_test::get_database_field_container());

        if (!optional_result)
        {
            BOOST_ERROR("Failed to select document");
            co_return;
        }

        const celeritas::redis_test loaded{ celeritas::database_type::redis, *optional_result };
        BOOST_CHECK_EQUAL(loaded.get_user_id(), user_id);
        BOOST_CHECK_EQUAL(loaded.get_chapter_id(), 10);
        BOOST_CHECK_EQUAL(loaded.get_chapter_name(), expected_chapter_name);
        BOOST_CHECK_EQUAL(loaded.get_currency(), expected_currency);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_update(const celeritas::redis_database_session_fixture& fixture, celeritas::redis_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        entity.set_chapter_name("Updated Chapter");
        entity.modify_currency(500);

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_all(const celeritas::redis_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_all_change = celeritas::redis_test::get_select(celeritas::database_type::redis);

        const auto results = co_await session->select_all(select_all_change, celeritas::redis_test::get_database_field_container());

        auto found = false;
        for (const auto& element : results)
        {
            if (const celeritas::redis_test test{ celeritas::database_type::redis, element };
                test.get_user_id() == user_id)
            {
                found = true;
                break;
            }
        }

        BOOST_CHECK(found);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_delete(const celeritas::redis_database_session_fixture& fixture, const celeritas::redis_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_delete(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_verify_delete(const celeritas::redis_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::redis_test::get_select(celeritas::database_type::redis, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::redis_test::get_database_field_container());

        BOOST_CHECK(!optional_result.has_value());
    }

    [[nodiscard]] celeritas::redis_test get_redis_test_for_user(const int64_t custom_user_id)
    {
        celeritas::redis_test entity{ celeritas::database_type::redis, custom_user_id };
        entity.set_chapter_id(10);
        entity.set_chapter_name("Test Chapter");
        entity.set_chance_winning(0.99);
        entity.set_winning(true);
        entity.set_currency(1000);
        entity.set_count(5);

        return entity;
    }

    [[nodiscard]] celeritas::redis_test get_redis_test()
    {
        return get_redis_test_for_user(user_id);
    }

    [[nodiscard]] celeritas::properties_data get_properties_data()
    {
        celeritas::properties_data properties{};
        properties.set_int64_value(987654321LL);
        properties.set_string_value("prop_string");
        properties.set_string_array_value({ "p1", "p2" });
        properties.set_int32_value(123);
        properties.set_int32_count_value(456);
        properties.set_int32_array_value({ 1, 2 });
        properties.set_int64_count_value(789LL);
        properties.set_int64_array_value({ 10LL, 20LL });
        properties.set_double_value(1.23);
        properties.set_double_array_value({ 1.1, 2.2 });
        properties.set_bool_value(true);
        return properties;
    }

    [[nodiscard]] celeritas::logs_data get_logs_data()
    {
        celeritas::logs_data log_data{};
        log_data.set_int64_value(123456789LL);
        log_data.set_string_value("log_string");
        log_data.set_string_array_value({ "l1", "l2" });
        log_data.set_int32_value(321);
        log_data.set_int32_count_value(654);
        log_data.set_int32_array_value({ 3, 4 });
        log_data.set_int64_count_value(987LL);
        log_data.set_int64_array_value({ 30LL, 40LL });
        log_data.set_double_value(3.21);
        log_data.set_double_array_value({ 3.3, 4.4 });
        log_data.set_bool_value(false);
        return log_data;
    }

    [[nodiscard]] celeritas::redis_test get_full_redis_test()
    {
        celeritas::redis_test entity{ celeritas::database_type::redis, user_id };
        entity.set_chapter_id(101);
        entity.set_chapter_name("Conversion Test");
        entity.set_chance_winning(0.5);
        entity.set_winning(false);
        entity.set_currency(2000);
        entity.set_count(20);
        entity.set_tags({ "a", "b", "c" });
        entity.set_category_index({ 1, 2, 3 });
        entity.set_related_index({ 10L, 20L, 30L });
        entity.set_ratios({ 0.1, 0.2, 0.3 });
        entity.set_attachment({ 'x', 'y', 'z' });

        const auto properties = get_properties_data();
        entity.set_properties(properties.to_document_type());

        celeritas::traits::document_array_type logs{};
        const auto log_data = get_logs_data();
        logs.emplace_back(log_data.to_document_type());
        entity.set_logs(logs);

        return entity;
    }

    void check_redis_test(const celeritas::redis_test& lhs, const celeritas::redis_test& rhs)
    {
        BOOST_CHECK_EQUAL(lhs.get_user_id(), rhs.get_user_id());
        BOOST_CHECK_EQUAL(lhs.get_chapter_id(), rhs.get_chapter_id());
        BOOST_CHECK_EQUAL(lhs.get_chapter_name(), rhs.get_chapter_name());
        BOOST_WARN_CLOSE(lhs.get_chance_winning(), rhs.get_chance_winning(), 0.00001);
        BOOST_CHECK_EQUAL(lhs.is_winning(), rhs.is_winning());
        BOOST_CHECK_EQUAL(lhs.get_currency(), rhs.get_currency());
        BOOST_CHECK_EQUAL(lhs.get_count(), rhs.get_count());

        celeritas::check_array(lhs.get_tags(), rhs.get_tags());
        celeritas::check_array(lhs.get_category_index(), rhs.get_category_index());
        celeritas::check_array(lhs.get_related_index(), rhs.get_related_index());
        celeritas::check_array(lhs.get_ratios(), rhs.get_ratios());
        celeritas::check_array(lhs.get_attachment(), rhs.get_attachment());

        check_properties_data(lhs.get_properties(), rhs.get_properties());
        check_logs_data(lhs.get_logs(), rhs.get_logs());
    }
}

BOOST_FIXTURE_TEST_SUITE(redis_database_session_suite, celeritas::redis_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_redis_database_session_constructor)
    {
        BOOST_CHECK(get_session());
    }

    BOOST_AUTO_TEST_CASE(test_connect)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                co_await get_session()->async_connect();
                BOOST_CHECK(co_await get_session()->is_health());
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR("Connection failed: " << error.what());
            }

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_is_health)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                co_await get_session()->async_connect();
                BOOST_CHECK(co_await get_session()->is_health());
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR("is_health failed: " << error.what());
            }

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_insert_and_select_one)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_redis_test();
                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);
                co_await test_select_one(*this, "Test Chapter", 1000);
                co_await test_delete(*this, entity);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_update_and_select_one)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_redis_test();
                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);
                co_await test_update(*this, entity);
                co_await test_select_one(*this, "Updated Chapter", 1500);
                co_await test_delete(*this, entity);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_delete_and_verify)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_redis_test();
                co_await test_insert(*this, entity);
                co_await test_delete(*this, entity);
                co_await test_verify_delete(*this);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_select_all_functionality)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_redis_test();
                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);
                co_await test_select_all(*this);
                co_await test_delete(*this, entity);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_select_all_with_no_entities)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                co_await test_delete(*this, get_redis_test_for_user(user_id));
                co_await test_delete(*this, get_redis_test_for_user(user_id + 1));
                co_await test_delete(*this, get_redis_test_for_user(user_id + 2));

                const auto select_all_change = celeritas::redis_test::get_select(celeritas::database_type::redis);
                const auto results = co_await session->select_all(select_all_change, celeritas::redis_test::get_database_field_container());

                BOOST_CHECK(results.empty());

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_select_all_with_multiple_entities)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity1 = get_redis_test_for_user(user_id + 1);
                auto entity2 = get_redis_test_for_user(user_id + 2);

                co_await test_delete(*this, entity1);
                co_await test_delete(*this, entity2);

                co_await test_insert(*this, entity1);
                co_await test_insert(*this, entity2);

                const auto select_all_change = celeritas::redis_test::get_select(celeritas::database_type::redis);
                const auto results = co_await session->select_all(select_all_change, celeritas::redis_test::get_database_field_container());

                auto found1 = false;
                auto found2 = false;
                for (const auto& element : results)
                {
                    const celeritas::redis_test test{ celeritas::database_type::redis, element };
                    if (test.get_user_id() == user_id + 1)
                    {
                        found1 = true;
                    }
                    else if (test.get_user_id() == user_id + 2)
                    {
                        found2 = true;
                    }
                }
                BOOST_CHECK(found1);
                BOOST_CHECK(found2);

                co_await test_delete(*this, entity1);
                co_await test_delete(*this, entity2);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_execute_changes_with_empty_change)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_redis_test();
                entity.clear_modify();
                co_await session->execute_changes(entity.get_modify(), 10);

                BOOST_CHECK(true);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_data_conversion_round_trip)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_full_redis_test();

                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);

                const auto select_change = celeritas::redis_test::get_select(celeritas::database_type::redis, user_id);
                const auto optional_result = co_await session->select_one(select_change, celeritas::redis_test::get_database_field_container());

                BOOST_REQUIRE(optional_result.has_value());
                const celeritas::redis_test loaded{ celeritas::database_type::redis, *optional_result };

                check_redis_test(loaded, entity);

                co_await test_delete(*this, entity);
                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_void)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto void_key = session->get_prefixed_key("test_void_key");
                const std::vector<std::string> set_command{ "SET", void_key, "void_value" };
                co_await session->async_execute_command_return_void(set_command);

                const std::vector<std::string> get_command{ "GET", void_key };
                const auto verify_void = co_await session->async_execute_command_return_optional_string(get_command);
                BOOST_REQUIRE(verify_void.has_value());
                BOOST_CHECK_EQUAL(*verify_void, "void_value");
                const std::vector<std::string> delete_command{ "DEL", void_key };
                co_await session->async_execute_command_return_int(delete_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_int)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto int_key = session->get_prefixed_key("test_int_key");
                const std::vector<std::string> set_int_command{ "SET", int_key, "10" };
                co_await session->async_execute_command_return_void(set_int_command);
                const std::vector<std::string> incr_command{ "INCR", int_key };
                const auto int_res = co_await session->async_execute_command_return_int(incr_command);
                BOOST_CHECK_EQUAL(int_res, 11);
                const std::vector<std::string> delete_int_command{ "DEL", int_key };
                co_await session->async_execute_command_return_int(delete_int_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_optional_string)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto str_key = session->get_prefixed_key("test_str_key");
                const std::vector<std::string> set_str_command{ "SET", str_key, "hello" };
                co_await session->async_execute_command_return_void(set_str_command);
                const std::vector<std::string> get_str_command{ "GET", str_key };
                const auto str_res = co_await session->async_execute_command_return_optional_string(get_str_command);
                BOOST_REQUIRE(str_res.has_value());
                BOOST_CHECK_EQUAL(*str_res, "hello");

                std::vector<std::string> get_str_none_command{ "GET", str_key + "_none" };
                const auto str_res_none = co_await session->async_execute_command_return_optional_string(get_str_none_command);
                BOOST_CHECK(!str_res_none.has_value());
                const std::vector<std::string> delete_str_command{ "DEL", str_key };
                co_await session->async_execute_command_return_int(delete_str_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_array_type)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto list_key = session->get_prefixed_key("test_list_key");
                const std::vector<std::string> r_push_command{ "RPUSH", list_key, "a", "b", "c" };
                co_await session->async_execute_command_return_int(r_push_command);
                const std::vector<std::string> l_range_command{ "LRANGE", list_key, "0", "-1" };
                const auto array_res = co_await session->async_execute_command_return_array(l_range_command);
                BOOST_REQUIRE_EQUAL(array_res.size(), 3);
                BOOST_CHECK_EQUAL(array_res[0], "a");
                BOOST_CHECK_EQUAL(array_res[1], "b");
                BOOST_CHECK_EQUAL(array_res[2], "c");
                const std::vector<std::string> delete_list_command{ "DEL", list_key };
                co_await session->async_execute_command_return_int(delete_list_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_map_type)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto hash_key = session->get_prefixed_key("test_hash_key");
                const std::vector<std::string> h_set_command = { "HSET", hash_key, "f1", "v1", "f2", "v2" };
                co_await session->async_execute_command_return_int(h_set_command);
                const std::vector<std::string> h_get_all_command = { "HGETALL", hash_key };
                auto map_res = co_await session->async_execute_command_return_map(h_get_all_command);
                BOOST_CHECK_EQUAL(map_res.size(), 2);
                BOOST_CHECK_EQUAL(map_res["f1"], "v1");
                BOOST_CHECK_EQUAL(map_res["f2"], "v2");
                const std::vector<std::string> del_hash_command = { "DEL", hash_key };
                co_await session->async_execute_command_return_int(del_hash_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_optional_double)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto z_set_key = session->get_prefixed_key("test_zset_key");
                const std::vector<std::string> z_add_command = { "ZADD", z_set_key, "1.5", "m1" };
                co_await session->async_execute_command_return_int(z_add_command);
                const std::vector<std::string> z_score_command = { "ZSCORE", z_set_key, "m1" };
                const auto double_res = co_await session->async_execute_command_return_optional_double(z_score_command);
                BOOST_REQUIRE(double_res.has_value());
                BOOST_CHECK_CLOSE(*double_res, 1.5, 0.0001);

                const std::vector<std::string> zscore_none_command = { "ZSCORE", z_set_key, "m_none" };
                const auto double_res_none = co_await session->async_execute_command_return_optional_double(zscore_none_command);
                BOOST_CHECK(!double_res_none.has_value());
                const std::vector<std::string> delete_z_set_command = { "DEL", z_set_key };
                co_await session->async_execute_command_return_int(delete_z_set_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_optional_int)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const auto z_rank_key = session->get_prefixed_key("test_zrank_key");
                const std::vector<std::string> z_add_rank_command = { "ZADD", z_rank_key, "10", "m1", "20", "m2" };
                co_await session->async_execute_command_return_int(z_add_rank_command);
                const std::vector<std::string> z_rank_command = { "ZRANK", z_rank_key, "m2" };
                const auto opt_int_res = co_await session->async_execute_command_return_optional_int(z_rank_command);
                BOOST_REQUIRE(opt_int_res.has_value());
                BOOST_CHECK_EQUAL(*opt_int_res, 1);

                const std::vector<std::string> z_rank_none_command = { "ZRANK", z_rank_key, "m_none" };
                const auto opt_int_res_none = co_await session->async_execute_command_return_optional_int(z_rank_none_command);
                BOOST_CHECK(!opt_int_res_none.has_value());
                const std::vector<std::string> delete_z_rank_command = { "DEL", z_rank_key };
                co_await session->async_execute_command_return_int(delete_z_rank_command);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_async_execute_command_return_scan_result)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("Redis not reachable, skipping test.");
                    co_return;
                }

                const std::vector<std::string> scan_command{ "SCAN", "0", "COUNT", "1" };
                const auto scan_res = co_await session->async_execute_command_return_scan_result(scan_command);
                BOOST_CHECK(!scan_res.get_cursor().empty());

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_get_prefixed_key)
    {
        const auto session = get_session();
        const std::string key{ "test_key" };
        const auto prefixed = session->get_prefixed_key(key);
        BOOST_CHECK(!prefixed.empty());
    }

    BOOST_AUTO_TEST_CASE(test_get_expire_seconds_command)
    {
        const auto session = get_session();
        const auto command = session->get_expire_seconds_command(120);
        BOOST_CHECK(!command.empty());
    }

    BOOST_AUTO_TEST_CASE(test_get_commands_accessors)
    {
        const auto session = get_session();
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_key_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_string_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_hash_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_list_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_set_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session->get_redis_sorted_set_commands()));
    }

BOOST_AUTO_TEST_SUITE_END()
