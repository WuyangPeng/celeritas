#include "database/basic/database_entity_change.h"
#include "database/session/mongo_database_session.h"
#include "database/generated/mongo/test/mongo_test.h"
#include "config/basic/database_type.h"
#include "config/aggregate/detail/database_config_reader.h"
#include "database/pool/database_pool_manager.h"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

// Global fixture for mongocxx initialization and cleanup
struct mongo_global_fixture
{
    mongo_global_fixture()
    {
        celeritas::database_pool_manager::create_mongo_instance();
    }

    ~mongo_global_fixture()
    {
        // The destructor of mongocxx::instance calls mongoc_cleanup().
    }
};

BOOST_GLOBAL_FIXTURE(mongo_global_fixture);

using namespace celeritas;

struct mongo_database_session_fixture
{
    boost::asio::io_context io_context;
    std::shared_ptr<mongo_database_session> session;
    std::shared_ptr<const database_config> config;
    bool test_end;

    mongo_database_session_fixture()
    {
        try
        {
            boost::filesystem::path config_path = boost::filesystem::current_path();
            while (!boost::filesystem::exists(config_path / "config"))
            {
                config_path = config_path.parent_path();
                if (config_path.empty() || !boost::filesystem::exists(config_path))
                {
                    throw std::runtime_error("Could not find project root containing 'config' directory.");
                }
            }
            config_path /= "config/tests/databases.xml";

            auto configs = database_config_reader::load_config(config_path.string());
            for (const auto& cfg : *configs)
            {
                if (cfg->get_database_type() == database_type::mongo)
                {
                    config = cfg;
                    break;
                }
            }

            if (!config)
            {
                throw std::runtime_error("Mongo config not found in " + config_path.string());
            }

            std::string uri = "mongodb://" + config->get_user() + ":" + config->get_password() + "@" + config->get_host() + ":" + std::to_string(config->get_port()) + "/" + config->get_db_name();

            session = std::make_shared<mongo_database_session>(
                config->get_host(),
                config->get_port(),
                config->get_user(),
                config->get_password(),
                uri,
                config->get_db_name(),
                config->get_expire_seconds(),
                io_context.get_executor()
                );
        }
        catch (const std::exception& e)
        {
            BOOST_TEST_MESSAGE("Failed to load database config: " << e.what());
            // Allow tests to proceed, they will likely fail on session creation or connect.
        }
    }

    ~mongo_database_session_fixture()
    {
        BOOST_CHECK(test_end);
    }

    void run(std::function<boost::asio::awaitable<void>()> func)
    {
        if (!session)
        {
            BOOST_FAIL("Session is not initialized, cannot run test.");
            return;
        }
        boost::asio::co_spawn(io_context, func, boost::asio::detached);
        io_context.run();
        io_context.restart();
    }
};

BOOST_FIXTURE_TEST_SUITE(mongo_database_session_suite, mongo_database_session_fixture)

    BOOST_AUTO_TEST_CASE(test_mongo_database_session_constructor)
    {
        BOOST_CHECK(session);
    }

    BOOST_AUTO_TEST_CASE(test_connect)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                co_await session->async_connect();
                bool health = co_await session->is_health();
                BOOST_TEST_MESSAGE("Database health: " << (health ? "UP" : "DOWN"));
            }
            catch (const std::exception& e)
            {
                BOOST_TEST_MESSAGE("Connection failed: " << e.what());
            }
        });
    }

    BOOST_AUTO_TEST_CASE(test_crud)
    {
        run([this]() -> boost::asio::awaitable<void> {
            try
            {
                co_await session->async_connect();
                if (!co_await session->is_health())
                {
                    BOOST_TEST_MESSAGE("MongoDB not reachable, skipping CRUD tests.");
                    co_return;
                }

                int64_t user_id = 123456789;
                mongo_test entity(database_type::mongo, user_id);
                entity.set_chapter_id(10);
                entity.set_chapter_name("Test Chapter");
                entity.set_chance_winning(0.99);
                entity.set_winning(true);
                entity.set_currency(1000);
                entity.set_count(5);

                co_await session->execute_changes(entity.get_delete(), config->get_expire_seconds());

                // 1. Insert (or Upsert)
                co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());
                entity.clear_modify();

                // 2. Select One
                auto select_change = mongo_test::get_select(database_type::mongo, user_id);
                auto result_opt = co_await session->select_one(select_change, mongo_test::get_database_field_container());

                if (!result_opt)
                {
                    BOOST_ERROR("Failed to select inserted document");
                    co_return;
                }

                mongo_test loaded(database_type::mongo, *result_opt);
                BOOST_CHECK_EQUAL(loaded.get_user_id(), user_id);
                BOOST_CHECK_EQUAL(loaded.get_chapter_id(), 10);
                BOOST_CHECK_EQUAL(loaded.get_chapter_name(), "Test Chapter");
                BOOST_CHECK_EQUAL(loaded.get_currency(), 1000);

                // 3. Update
                entity.set_chapter_name("Updated Chapter");
                entity.modify_currency(500); // 1000 + 500 = 1500
                co_await session->execute_changes(entity.get_modify(), config->get_expire_seconds());

                result_opt = co_await session->select_one(select_change, mongo_test::get_database_field_container());
                if (!result_opt)
                {
                    BOOST_ERROR("Failed to select updated document");
                    co_return;
                }

                mongo_test updated(database_type::mongo, *result_opt);
                BOOST_CHECK_EQUAL(updated.get_chapter_name(), "Updated Chapter");
                BOOST_CHECK_EQUAL(updated.get_currency(), 1500);

                // 4. Select All
                auto select_all_change = mongo_test::get_select(database_type::mongo);
                auto results = co_await session->select_all(select_all_change, mongo_test::get_database_field_container());

                bool found = false;
                for (const auto& res : results)
                {
                    mongo_test t(database_type::mongo, res);
                    if (t.get_user_id() == user_id)
                    {
                        found = true;
                        break;
                    }
                }
                BOOST_CHECK(found);

                // 5. Delete
                co_await session->execute_changes(entity.get_delete(), config->get_expire_seconds());

                result_opt = co_await session->select_one(select_change, mongo_test::get_database_field_container());
                BOOST_CHECK(!result_opt.has_value());

                test_end = true;
            }
            catch (const std::exception& e)
            {
                BOOST_ERROR(std::string("Exception in CRUD test: ") + e.what());
            }
        });
    }

BOOST_AUTO_TEST_SUITE_END()
