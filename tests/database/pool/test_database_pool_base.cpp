#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/pool/database_pool_base.h"
#include "mock/mock_database_pool_base.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
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

    BOOST_AUTO_TEST_CASE(test_execute_changes_forwarding)
    {
        boost::asio::io_context io_context{};
        const auto pool = std::make_shared<celeritas::mock_database_pool_base>();

        boost::asio::co_spawn(io_context,
                              [&]() -> boost::asio::awaitable<void> {
                                  const auto dummy_change = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type);
                                  const auto result = co_await pool->execute_changes(dummy_change, 0);
                                  BOOST_TEST(result);
                                  BOOST_TEST(pool->is_execute_changes_called());
                                  BOOST_TEST(pool->get_expiration_time_passed() == 0);
                              },
                              boost::asio::detached);

        io_context.run();
    }

BOOST_AUTO_TEST_SUITE_END()
