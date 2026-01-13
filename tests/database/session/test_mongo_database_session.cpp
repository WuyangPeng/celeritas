#include "config/basic/database_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/generated/mongo/test/mongo_test.h"
#include "database/session/mongo_database_session.h"
#include "fixture/mongo_database_session_fixture.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

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

    BOOST_AUTO_TEST_CASE(test_crud)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                const auto session = get_session();
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_ERROR("MongoDB not reachable, skipping CRUD tests.");
                    co_return;
                }

                auto entity = get_mongo_test();

                co_await test_delete(*this, entity);

                co_await test_insert(*this, entity);
                co_await test_select_one(*this, "Test Chapter", 1000);

                co_await test_update(*this, entity);
                co_await test_select_one(*this, "Updated Chapter", 1500);
                co_await test_select_all(*this);

                co_await test_delete(*this, entity);
                co_await test_verify_delete(*this);

                set_test_end(true);
            }
            catch (const std::exception& error)
            {
                BOOST_ERROR(std::string{"Exception in CRUD test: "} + error.what());
            }
        });
    }

BOOST_AUTO_TEST_SUITE_END()
