#include "database/pool/database_pool_manager.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_state.h"
#include "player/online/player_online_component.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(player_online_component_suite)

    struct test_fixture
    {
        test_fixture()
            : io_context_{},
              player_state_{ boost::asio::make_strand(io_context_) },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              online_component_{ &player_state_ }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);
        }

        ~test_fixture()
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(nullptr);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        celeritas::player_online_component online_component_;
    };

    BOOST_FIXTURE_TEST_CASE(test_load_marks_modified, test_fixture)
    {
        boost::asio::co_spawn(io_context_, online_component_.on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(online_component_.is_modify());
    }

    BOOST_FIXTURE_TEST_CASE(test_save_clears_modify, test_fixture)
    {
        boost::asio::co_spawn(io_context_, online_component_.on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(online_component_.is_modify());

        boost::asio::co_spawn(io_context_, online_component_.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!online_component_.is_modify());
    }

BOOST_AUTO_TEST_SUITE_END()
