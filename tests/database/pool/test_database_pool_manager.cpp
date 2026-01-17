#include "common/core/celeritas_error.h"
#include "config/aggregate/detail/database_config_reader.h"
#include "config/local/database_config.h"
#include "database/pool/connection_pool.tpp"
#include "database/pool/database_pool_manager.h"
#include "database/session/mongo_database_session.h"
#include "database/session/mysql_database_session.h"
#include "database/session/redis_database_session.h"
#include "fixture/connection_pool_fixture.h"
#include "mock/mock_database_pool_base.h"

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    [[nodiscard]] std::shared_ptr<const celeritas::database_config> get_db_config(const celeritas::database_type type)
    {
        const auto current_path = boost::filesystem::current_path() / "config/tests/databases.xml";

        for (const auto configs = celeritas::database_config_reader::load_config(current_path.string());
             const auto& config : *configs)
        {
            if (config->get_database_type() == type)
            {
                return config;
            }
        }
        throw celeritas::celeritas_error{ "Database config not found for the specified type." };
    }

    [[nodiscard]] auto create_test_pool(const std::string& pool_name,
                                        const celeritas::database_type db_type,
                                        const boost::asio::any_io_executor& executor)
    {
        const auto config = get_db_config(db_type);
        return celeritas::database_pool_manager::get_instance().create_pool(pool_name,
                                                                            db_type,
                                                                            executor,
                                                                            config->get_host(),
                                                                            config->get_port(),
                                                                            config->get_user(),
                                                                            config->get_password(),
                                                                            config->get_db_name(),
                                                                            config->get_min_connections(),
                                                                            config->get_max_connections(),
                                                                            config->get_expire_seconds());
    }
}

BOOST_AUTO_TEST_SUITE(database_pool_manager_suite)

    BOOST_AUTO_TEST_CASE(test_get_instance)
    {
        auto& instance1 = celeritas::database_pool_manager::get_instance();
        auto& instance2 = celeritas::database_pool_manager::get_instance();
        BOOST_TEST(&instance1 == &instance2);
    }

    BOOST_AUTO_TEST_CASE(test_create_and_get_mongo_pool)
    {
        celeritas::database_pool_manager::create_mongo_instance();

        boost::asio::io_context io_context{};

        const auto pool_name = "test_mongo_pool";
        auto pool = create_test_pool(pool_name, celeritas::database_type::mongo, io_context.get_executor());

        BOOST_TEST(pool != nullptr);

        const auto retrieved_pool = celeritas::database_pool_manager::get_instance().get_pool(pool_name);
        BOOST_TEST(pool == retrieved_pool);

        celeritas::database_pool_manager::get_instance().release_pool();
    }

    BOOST_AUTO_TEST_CASE(test_create_and_get_mysql_pool)
    {
        boost::asio::io_context io_context{};

        const auto pool_name = "test_mysql_pool";
        auto pool = create_test_pool(pool_name, celeritas::database_type::mysql, io_context.get_executor());

        BOOST_TEST(pool != nullptr);

        const auto retrieved_pool = celeritas::database_pool_manager::get_instance().get_pool(pool_name);
        BOOST_TEST(pool == retrieved_pool);

        celeritas::database_pool_manager::get_instance().release_pool();
    }

    BOOST_AUTO_TEST_CASE(test_create_and_get_redis_pool)
    {
        boost::asio::io_context io_context{};

        const auto pool_name = "test_redis_pool";
        auto pool = create_test_pool(pool_name, celeritas::database_type::redis, io_context.get_executor());

        BOOST_TEST(pool != nullptr);

        const auto retrieved_pool = celeritas::database_pool_manager::get_instance().get_pool(pool_name);
        BOOST_TEST(pool == retrieved_pool);

        celeritas::database_pool_manager::get_instance().release_pool();
    }

    BOOST_FIXTURE_TEST_CASE(test_is_health, celeritas::connection_pool_fixture)
    {
        celeritas::database_pool_manager::create_mongo_instance();

        const auto pool_name = "test_health_mongo_pool";
        auto pool = create_test_pool(pool_name, celeritas::database_type::mongo, get_io_context().get_executor());

        run([pool = std::move(pool),this]() -> boost::asio::awaitable<void> {
            co_await pool->async_initialize();
            BOOST_TEST(co_await celeritas::database_pool_manager::get_instance().is_health());
            celeritas::database_pool_manager::get_instance().release_pool();

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_release_pool)
    {
        celeritas::database_pool_manager::create_mongo_instance();

        const auto pool_name = "test_release_pool";
        boost::asio::io_context io_context{};

        const auto pool = create_test_pool(pool_name, celeritas::database_type::mongo, io_context.get_executor());

        BOOST_TEST(celeritas::database_pool_manager::get_instance().get_pool(pool_name) != nullptr);
        celeritas::database_pool_manager::get_instance().release_pool();
        BOOST_CHECK_THROW([pool_name ]{std::ignore = celeritas::database_pool_manager::get_instance().get_pool(pool_name); }(),
                          celeritas::celeritas_error);
    }

    BOOST_FIXTURE_TEST_CASE(test_set_mock_pool, celeritas::connection_pool_fixture)
    {
        const auto mock_pool = std::make_shared<celeritas::mock_database_pool_base>();
        celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool);

        run([this]() -> boost::asio::awaitable<void> {
            BOOST_TEST(co_await celeritas::database_pool_manager::get_instance().is_health());

            set_test_end(true);
        });

        celeritas::database_pool_manager::get_instance().set_mock_pool(nullptr);
    }

    BOOST_AUTO_TEST_CASE(test_start_cleanup_timer)
    {
        boost::asio::io_context io_context{};

        auto mock_pool = std::make_shared<celeritas::mock_database_pool_base>();

        BOOST_CHECK_NO_THROW(celeritas::database_pool_manager::get_instance().start_cleanup_timer(io_context.get_executor()));

        io_context.run_for(std::chrono::milliseconds(100));

        celeritas::database_pool_manager::get_instance().release_pool();
    }

BOOST_AUTO_TEST_SUITE_END()
