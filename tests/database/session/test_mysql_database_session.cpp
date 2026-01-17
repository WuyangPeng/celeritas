#include "config/basic/database_type.h"
#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.tpp"
#include "database/detail/check_helper.h"
#include "database/document/test/logs_data.h"
#include "database/document/test/properties_data.h"
#include "database/generated/mysql/test/mysql_test.h"
#include "database/session/mysql_database_session.h"
#include "fixture/database_session_fixture.tpp"
#include "fixture/mysql_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <memory>
#include <vector>

namespace
{
    constexpr auto user_id = 123456789;

    [[nodiscard]] boost::asio::awaitable<void> test_insert(const celeritas::mysql_database_session_fixture& fixture, celeritas::mysql_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
        entity.clear_modify();
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_one(const celeritas::mysql_database_session_fixture& fixture, const std::string& expected_chapter_name, const int64_t expected_currency)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::mysql_test::get_select(celeritas::database_type::mysql, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::mysql_test::get_database_field_container());

        if (!optional_result)
        {
            BOOST_ERROR("Failed to select document");
            co_return;
        }

        const celeritas::mysql_test loaded{ celeritas::database_type::mysql, *optional_result };
        BOOST_CHECK_EQUAL(loaded.get_user_id(), user_id);
        BOOST_CHECK_EQUAL(loaded.get_chapter_id(), 10);
        BOOST_CHECK_EQUAL(loaded.get_chapter_name(), expected_chapter_name);
        BOOST_CHECK_EQUAL(loaded.get_currency(), expected_currency);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_update(const celeritas::mysql_database_session_fixture& fixture, celeritas::mysql_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        entity.set_chapter_name("Updated Chapter");
        entity.modify_currency(500);

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_all(const celeritas::mysql_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_all_change = celeritas::mysql_test::get_select(celeritas::database_type::mysql);
        const auto results = co_await session->select_all(select_all_change, celeritas::mysql_test::get_database_field_container());

        auto found = false;
        for (const auto& element : results)
        {
            if (const celeritas::mysql_test test{ celeritas::database_type::mysql, element };
                test.get_user_id() == user_id)
            {
                found = true;
                break;
            }
        }

        BOOST_CHECK(found);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_delete(const celeritas::mysql_database_session_fixture& fixture, const celeritas::mysql_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_delete(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_verify_delete(const celeritas::mysql_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::mysql_test::get_select(celeritas::database_type::mysql, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::mysql_test::get_database_field_container());

        BOOST_CHECK(!optional_result.has_value());
    }

    [[nodiscard]] celeritas::mysql_test get_mysql_test()
    {
        celeritas::mysql_test entity{ celeritas::database_type::mysql, user_id };
        entity.set_chapter_id(10);
        entity.set_chapter_name("Test Chapter");
        entity.set_chance_winning(0.99);
        entity.set_winning(true);
        entity.set_currency(1000);
        entity.set_count(5);

        return entity;
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

    [[nodiscard]] celeritas::mysql_test get_full_mysql_test()
    {
        celeritas::mysql_test entity{ celeritas::database_type::mysql, user_id };
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

        const auto properties = get_properties_data();
        entity.set_properties(properties.to_document_type());

        celeritas::traits::document_array_type logs{};
        const auto log_data = get_logs_data();
        logs.emplace_back(log_data.to_document_type());
        entity.set_logs(logs);

        return entity;
    }

    void check_mysql_test(const celeritas::mysql_test& lhs, const celeritas::mysql_test& rhs)
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

        check_properties_data(lhs.get_properties(), rhs.get_properties());
        check_logs_data(lhs.get_logs(), rhs.get_logs());
    }
}

BOOST_FIXTURE_TEST_SUITE(mysql_database_session_suite, celeritas::mysql_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_mysql_database_session_constructor)
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mysql_test();
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mysql_test();
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mysql_test();
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mysql_test();
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

    BOOST_AUTO_TEST_CASE(test_async_query)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("MySQL not reachable, skipping async_query test.");
                    co_return;
                }

                auto entity = get_mysql_test();
                co_await test_insert(*this, entity);

                const auto sql = "SELECT * FROM mysql_test WHERE user_id = " + std::to_string(user_id);
                const auto results = co_await session->async_query(sql);

                BOOST_CHECK(results.has_value());
                BOOST_CHECK_EQUAL(results.rows().size(), 1);

                co_await test_delete(*this, entity);
                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in async_query test: "} + error.what());
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mysql_test();
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
                    BOOST_ERROR("MySQL not reachable, skipping test.");
                    co_return;
                }
                auto entity = get_full_mysql_test();

                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);

                const auto select_change = celeritas::mysql_test::get_select(celeritas::database_type::mysql, user_id);
                const auto optional_result = co_await session->select_one(select_change, celeritas::mysql_test::get_database_field_container());

                BOOST_REQUIRE(optional_result.has_value());
                const celeritas::mysql_test loaded{ celeritas::database_type::mysql, *optional_result };

                check_mysql_test(loaded, entity);

                co_await test_delete(*this, entity);
                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in test: "} + error.what());
            }
        });
    }

BOOST_AUTO_TEST_SUITE_END()
