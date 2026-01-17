#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/pool/database_pool_base.h"
#include "fixture/connection_pool_fixture.h"
#include "mock/mock_database_pool_base.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_pool_base_suite)

    BOOST_AUTO_TEST_CASE(test_cleanup_timer)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::mock_database_pool_base>();

        pool->start_cleanup_timer(io_context.get_executor(), std::chrono::milliseconds(1000));

        io_context.run_for(std::chrono::milliseconds(1500));
        BOOST_TEST(pool->get_cleanup_called_count() > 0);

        pool->stop_cleanup_timer();
        auto previous_count = pool->get_cleanup_called_count();
        io_context.restart();
        io_context.run_for(std::chrono::milliseconds(1500));

        BOOST_TEST(pool->get_cleanup_called_count() == previous_count);
    }

    BOOST_FIXTURE_TEST_CASE(test_execute_changes_forwarding, celeritas::connection_pool_fixture)
    {
        run([&]() -> boost::asio::awaitable<void> {
            const auto pool = std::make_shared<celeritas::mock_database_pool_base>();
            const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
            const auto dummy_change = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                                "test",
                                                                                                celeritas::database_change_type::insert_type,
                                                                                                key_container);
            const auto result = co_await pool->execute_changes(dummy_change, 2);

            BOOST_TEST(result);
            BOOST_TEST(pool->is_execute_changes_called());
            BOOST_TEST(pool->get_expiration_time_passed() == 2);

            set_test_end(true);
        });
    }

BOOST_AUTO_TEST_SUITE_END()
