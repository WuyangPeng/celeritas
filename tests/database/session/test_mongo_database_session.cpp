#include "config/basic/database_type.h"
#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.tpp"
#include "database/document/test/logs_data.h"
#include "database/document/test/properties_data.h"
#include "database/generated/mongo/test/mongo_test.h"
#include "database/session/mongo_database_session.h"
#include "fixture/mongo_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <map>
#include <memory>
#include <vector>

namespace
{
    constexpr auto user_id = 123456789;

    [[nodiscard]] boost::asio::awaitable<void> test_insert(const celeritas::mongo_database_session_fixture& fixture, celeritas::mongo_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
        entity.clear_modify();
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_one(const celeritas::mongo_database_session_fixture& fixture, const std::string& expected_chapter_name, const int64_t expected_currency)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::mongo_test::get_select(celeritas::database_type::mongo, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::mongo_test::get_database_field_container());

        if (!optional_result)
        {
            BOOST_ERROR("Failed to select document");
            co_return;
        }

        const celeritas::mongo_test loaded{ celeritas::database_type::mongo, *optional_result };
        BOOST_CHECK_EQUAL(loaded.get_user_id(), user_id);
        BOOST_CHECK_EQUAL(loaded.get_chapter_id(), 10);
        BOOST_CHECK_EQUAL(loaded.get_chapter_name(), expected_chapter_name);
        BOOST_CHECK_EQUAL(loaded.get_currency(), expected_currency);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_update(const celeritas::mongo_database_session_fixture& fixture, celeritas::mongo_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        entity.set_chapter_name("Updated Chapter");
        entity.modify_currency(500);

        co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_select_all(const celeritas::mongo_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_all_change = celeritas::mongo_test::get_select(celeritas::database_type::mongo);
        const auto results = co_await session->select_all(select_all_change, celeritas::mongo_test::get_database_field_container());

        auto found = false;
        for (const auto& element : results)
        {
            if (const celeritas::mongo_test test{ celeritas::database_type::mongo, element };
                test.get_user_id() == user_id)
            {
                found = true;
                break;
            }
        }

        BOOST_CHECK(found);
    }

    [[nodiscard]] boost::asio::awaitable<void> test_delete(const celeritas::mongo_database_session_fixture& fixture, const celeritas::mongo_test& entity)
    {
        const auto session = fixture.get_session();
        const auto config = fixture.get_config();

        co_await session->execute_changes(entity.get_delete(), config->get_expire_seconds());
    }

    [[nodiscard]] boost::asio::awaitable<void> test_verify_delete(const celeritas::mongo_database_session_fixture& fixture)
    {
        const auto session = fixture.get_session();
        const auto select_change = celeritas::mongo_test::get_select(celeritas::database_type::mongo, user_id);
        const auto optional_result = co_await session->select_one(select_change, celeritas::mongo_test::get_database_field_container());

        BOOST_CHECK(!optional_result.has_value());
    }

    [[nodiscard]] celeritas::mongo_test get_mongo_test()
    {
        celeritas::mongo_test entity{ celeritas::database_type::mongo, user_id };
        entity.set_chapter_id(10);
        entity.set_chapter_name("Test Chapter");
        entity.set_chance_winning(0.99);
        entity.set_winning(true);
        entity.set_currency(1000);
        entity.set_count(5);

        return entity;
    }
}

BOOST_FIXTURE_TEST_SUITE(mongo_database_session_suite, celeritas::mongo_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_mongo_database_session_constructor)
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mongo_test();
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mongo_test();
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mongo_test();
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mongo_test();
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

    BOOST_AUTO_TEST_CASE(test_async_find)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("MongoDB not reachable, skipping async_find test.");
                    co_return;
                }

                auto entity = get_mongo_test();
                co_await test_insert(*this, entity);

                bsoncxx::builder::basic::document filter_builder{};
                filter_builder.append(bsoncxx::builder::basic::kvp("_id", static_cast<int64_t>(user_id)));

                auto cursor = co_await session->async_find("mongo_test", filter_builder.view());

                auto count = 0;
                for (const auto& doc : cursor)
                {
                    BOOST_CHECK(doc["_id"].get_int64() == user_id);
                    count++;
                }
                BOOST_CHECK_EQUAL(count, 1);

                co_await test_delete(*this, entity);
                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in async_find test: "} + error.what());
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                auto entity = get_mongo_test();
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
                    BOOST_ERROR("MongoDB not reachable, skipping test.");
                    co_return;
                }

                celeritas::mongo_test entity{ celeritas::database_type::mongo, user_id };
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

                celeritas::properties_data properties{ 987654321LL };
                properties.set_string_val("prop_string");
                properties.set_string_array_val({ "p1", "p2" });
                properties.set_int32_val(123);
                properties.set_int32_count_val(456);
                properties.set_int32_array_val({ 1, 2 });
                properties.set_int64_count_val(789LL);
                properties.set_int64_array_val({ 10LL, 20LL });
                properties.set_double_val(1.23);
                properties.set_double_array_val({ 1.1, 2.2 });
                properties.set_bool_val(true);
                entity.set_properties(properties.to_document_type());

                celeritas::traits::document_array_type logs{};
                celeritas::logs_data log_data{ 123456789LL };
                log_data.set_string_val("log_string");
                log_data.set_string_array_val({ "l1", "l2" });
                log_data.set_int32_val(321);
                log_data.set_int32_count_val(654);
                log_data.set_int32_array_val({ 3, 4 });
                log_data.set_int64_count_val(987LL);
                log_data.set_int64_array_val({ 30LL, 40LL });
                log_data.set_double_val(3.21);
                log_data.set_double_array_val({ 3.3, 4.4 });
                log_data.set_bool_val(false);
                logs.emplace_back(log_data.to_document_type());
                entity.set_logs(logs);

                co_await test_delete(*this, entity);
                co_await test_insert(*this, entity);

                const auto select_change = celeritas::mongo_test::get_select(celeritas::database_type::mongo, user_id);
                const auto optional_result = co_await session->select_one(select_change, celeritas::mongo_test::get_database_field_container());

                BOOST_REQUIRE(optional_result.has_value());
                const celeritas::mongo_test loaded{ celeritas::database_type::mongo, *optional_result };

                BOOST_CHECK_EQUAL(loaded.get_user_id(), user_id);
                BOOST_CHECK_EQUAL(loaded.get_chapter_id(), 101);
                BOOST_CHECK_EQUAL(loaded.get_chapter_name(), "Conversion Test");
                BOOST_CHECK_EQUAL(loaded.get_chance_winning(), 0.5);
                BOOST_CHECK_EQUAL(loaded.is_winning(), false);
                BOOST_CHECK_EQUAL(loaded.get_currency(), 2000);
                BOOST_CHECK_EQUAL(loaded.get_count(), 20);

                const auto& tags = loaded.get_tags();
                BOOST_CHECK_EQUAL(tags.size(), 3);
                BOOST_CHECK_EQUAL(tags[0], "a");

                const auto& categories = loaded.get_category_index();
                BOOST_CHECK_EQUAL(categories.size(), 3);
                BOOST_CHECK_EQUAL(categories[1], 2);

                const auto& related = loaded.get_related_index();
                BOOST_CHECK_EQUAL(related.size(), 3);
                BOOST_CHECK_EQUAL(related[2], 30L);

                const auto& ratios = loaded.get_ratios();
                BOOST_CHECK_EQUAL(ratios.size(), 3);
                BOOST_CHECK_EQUAL(ratios[0], 0.1);

                const auto& attachment = loaded.get_attachment();
                BOOST_CHECK_EQUAL(attachment.size(), 3);
                BOOST_CHECK_EQUAL(attachment[1], 'y');

                const auto& loaded_properties_doc = loaded.get_properties();
                const auto loaded_properties = celeritas::properties_data::from_document(loaded_properties_doc);
                BOOST_CHECK_EQUAL(loaded_properties.get_expire_time(), 987654321L);
                BOOST_CHECK_EQUAL(loaded_properties.get_string_val(), "prop_string");
                BOOST_CHECK_EQUAL(loaded_properties.get_string_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_properties.get_string_array_val()[0], "p1");
                BOOST_CHECK_EQUAL(loaded_properties.get_int32_val(), 123);
                BOOST_CHECK_EQUAL(loaded_properties.get_int32_count_val(), 456);
                BOOST_CHECK_EQUAL(loaded_properties.get_int32_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_properties.get_int32_array_val()[0], 1);
                BOOST_CHECK_EQUAL(loaded_properties.get_int64_count_val(), 789LL);
                BOOST_CHECK_EQUAL(loaded_properties.get_int64_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_properties.get_int64_array_val()[0], 10LL);
                BOOST_CHECK_EQUAL(loaded_properties.get_double_val(), 1.23);
                BOOST_CHECK_EQUAL(loaded_properties.get_double_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_properties.get_double_array_val()[0], 1.1);
                BOOST_CHECK_EQUAL(loaded_properties.get_bool_val(), true);

                const auto& loaded_logs_doc = loaded.get_logs();
                BOOST_REQUIRE_EQUAL(loaded_logs_doc.size(), 1);
                const auto loaded_log = celeritas::logs_data::from_document(loaded_logs_doc.at(0));
                BOOST_CHECK_EQUAL(loaded_log.get_expire_time(), 123456789L);
                BOOST_CHECK_EQUAL(loaded_log.get_string_val(), "log_string");
                BOOST_CHECK_EQUAL(loaded_log.get_string_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_log.get_string_array_val()[0], "l1");
                BOOST_CHECK_EQUAL(loaded_log.get_int32_val(), 321);
                BOOST_CHECK_EQUAL(loaded_log.get_int32_count_val(), 654);
                BOOST_CHECK_EQUAL(loaded_log.get_int32_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_log.get_int32_array_val()[0], 3);
                BOOST_CHECK_EQUAL(loaded_log.get_int64_count_val(), 987LL);
                BOOST_CHECK_EQUAL(loaded_log.get_int64_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_log.get_int64_array_val()[0], 30LL);
                BOOST_CHECK_EQUAL(loaded_log.get_double_val(), 3.21);
                BOOST_CHECK_EQUAL(loaded_log.get_double_array_val().size(), 2);
                BOOST_CHECK_EQUAL(loaded_log.get_double_array_val()[0], 3.3);
                BOOST_CHECK_EQUAL(loaded_log.get_bool_val(), false);

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
